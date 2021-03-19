/*
* Copyright (C) 2017 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     zhangdongdong <zhangdongdong@uniontech.com>
*
* Maintainer: zhangdongdong <zhangdongdong@uniontech.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "pwqualitymanager.h"

#include <QStringList>
#include <QDebug>
#include <QSettings>
#include <QApplication>
#include <QtGlobal>
#include <QFileInfo>

#include "window/utils.h"

PwqualityManager::PwqualityManager()
    : m_passwordMinLength(-1)
    , m_passwordMaxLength(-1)
    , m_validateRequiredString(-1)
{
}

PwqualityManager *PwqualityManager::instance()
{
    static PwqualityManager pwquality;
    return &pwquality;
}

bool PwqualityManager::palindromeChecked(const QString &text)
{
    QStringList list;
    for (int pos = 0; pos < text.size() + 1 - m_palindromeLength; pos++) {
        list.append(text.mid(pos, m_palindromeLength));
    }

    // 判断是否是连续4个字符的回文,如果是就返回false
    for (QString str : list) {
        bool isPalindrome = true;
        for (int i = 0; i < m_palindromeLength / 2; i++) {
            if (str[i] == str[m_palindromeLength - 1 - i]) {
                continue;
            } else {
                isPalindrome = false;
            }
        }
        if (isPalindrome) return false;
    }

    return true;
}

QString PwqualityManager::dictChecked(const QString &text)
{
    QFile file("/usr/share/dict/MainEnglishDictionary_ProbWL.txt");
    if (!file.open(QIODevice::Text | QIODevice::ReadOnly)) {
        qDebug() << "dict file not found, skip check";
        return QString();
    }

    QStringList dictList;
    QTextStream out(&file);
    while (!out.atEnd()) {
        dictList.append(out.readLine());
    }

    if (!dictList.contains(text))
        return QString();

    return QString("error password");
}

int PwqualityManager::verifyPassword(const QString &password)
{
    QFileInfo fileInfo("/etc/deepin/dde.conf");
    if (fileInfo.isFile()) {
        // NOTE(justforlxz): 配置文件由安装器生成，后续改成PAM模块
        QSettings setting("/etc/deepin/dde.conf", QSettings::IniFormat);
        setting.beginGroup("Password");
        const bool strong_password_check = setting.value("STRONG_PASSWORD", false).toBool();
        m_passwordMinLength = setting.value("PASSWORD_MIN_LENGTH").toInt();
        m_passwordMaxLength = setting.value("PASSWORD_MAX_LENGTH").toInt();
        m_validateRequiredString = setting.value("VALIDATE_REQUIRED").toInt();
        QString validate_policy_string = setting.value("VALIDATE_POLICY").toString();
        if (validate_policy_string.isEmpty()) {
            validate_policy_string = R"(1234567890;abcdefghijklmnopqrstuvwxyz;ABCDEFGHIJKLMNOPQRSTUVWXYZ;~`!@#$%^&*()-_+=|\{}[]:"'<>,.?/)";
        }
        const QStringList validate_policy = validate_policy_string.split(";");
        int max_repeat = setting.value("maxrepeat").toInt();
        int max_sequence = setting.value("maxsequence").toInt();
        int ucredit = setting.value("ucredit").toInt();
        int ocredit = setting.value("ocredit").toInt();

        if (!strong_password_check) {
            return ENUM_PASSWORD_CHARACTER;
        }
        if (password.size() == 0) {
            return ENUM_PASSWORD_NOTEMPTY;
        }
        if (passwordCompositionType(validate_policy, password) < m_validateRequiredString) {
            if (password.size() < m_passwordMinLength) {
                return ENUM_PASSWORD_SEVERAL;
            }
            if (!(password.split("").toSet() - validate_policy.join("").split("").toSet()).isEmpty()) {
                return ENUM_PASSWORD_CHARACTER;
            }
            return ENUM_PASSWORD_TYPE;
        }
        if (password.size() < m_passwordMinLength) {
            return ENUM_PASSWORD_TOOSHORT;
        }
        if (password.size() > m_passwordMaxLength) {
            return ENUM_PASSWORD_TOOLONG;
        }
        if (!containsChar(password, validate_policy_string)) {
            return ENUM_PASSWORD_CHARACTER;
        }
        if (dccV20::IsServerSystem) {
            if (!PwqualityManager::instance()->palindromeChecked(password)) {
                return ENUM_PASSWORD_PALINDROME;
            }

            QString sChkResult = PwqualityManager::instance()->dictChecked(password);
            if (!sChkResult.isEmpty()) {
                return ENUM_PASSWORD_DICT_FORBIDDEN;
            }
            return ENUM_PASSWORD_SUCCESS;
        }
        if (!duplicateCheck(password, max_repeat)) {
            return ENUM_PASSWORD_DICT_FORBIDDEN;
        }
        if (!continuousCheck(password, max_sequence)) {
            return ENUM_PASSWORD_DICT_FORBIDDEN;
        }
        QPair<int, int> result = SpecialCharCount(password, validate_policy.at(2), validate_policy.at(3));
        if (result.first < ucredit || result.second < ocredit) {
            return ENUM_PASSWORD_DICT_FORBIDDEN;
        }
        return ENUM_PASSWORD_SUCCESS;
    } else {
        QString validate_policy = QString("1234567890") + QString("abcdefghijklmnopqrstuvwxyz") +
                                      QString("ABCDEFGHIJKLMNOPQRSTUVWXYZ") + QString("~!@#$%^&*()[]{}\\|/?,.<>");
        qDebug() << "configuration file not exist";
        bool ret = containsChar(password, validate_policy);
        if (!ret) {
            return ENUM_PASSWORD_CHARACTER;
        }
        return ENUM_PASSWORD_SUCCESS;
    }
}

int  PwqualityManager::passwordCompositionType(const QStringList &validate, const QString &password)
{
    return static_cast<int>(std::count_if(validate.cbegin(), validate.cend(),
                                          [=](const QString &policy) {
                                              for (const QChar &c : policy) {
                                                  if (password.contains(c)) {
                                                      return true;
                                                  }
                                              }
                                              return false;
                                          }));
}

bool PwqualityManager::containsChar(const QString &password, const QString &validate)
{
    for (const QChar &p : password) {
        if (!validate.contains(p)) {
            return false;
        }
    }

    return true;
}

bool PwqualityManager::duplicateCheck(const QString &password, const int count)
{
    int duplicateCount(0);
    for (int i = 0; i < password.size(); ++i) {
        if (i == 0) continue;
        if (password.at(i) == password.at(i - 1)) {
            duplicateCount++;
        } else {
            duplicateCount = 0;
        }
        if (duplicateCount >= count) {
            return false;
        }
    }
    return true;
}

bool PwqualityManager::continuousCheck(const QString &password, const int count)
{
    int continuousCount(0);
    for (int i = 0; i < password.size(); ++i) {
        if (i == 0) continue;
        if (password.at(i).toLatin1() - password.at(i - 1).toLatin1() == 1) {
            continuousCount++;
        } else {
            continuousCount = 0;
        }
        if (continuousCount >= count) {
            return false;
        }
    }
    return true;
}

QPair<int, int> PwqualityManager::SpecialCharCount(const QString &password, const QString &upper, const QString &symbol)
{
    int upperCount(0);
    int symbolCount(0);

    for(auto c : password) {
        if (upper.contains(c)) upperCount++;
        if (symbol.contains(c)) symbolCount++;
    }

    return QPair<int, int>(upperCount, symbolCount);
}

