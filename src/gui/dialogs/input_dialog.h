/*  Copyright (C) 2014-2016 FastoGT. All right reserved.

    This file is part of FastoNoSQL.

    FastoNoSQL is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    FastoNoSQL is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FastoNoSQL.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma  once

#include <QDialog>

class QLineEdit;

namespace fastonosql {

class InputDialog
        : public QDialog {
  Q_OBJECT
 public:
  enum InputType { SingleLine, DoubleLine };
  explicit InputDialog(QWidget* parent, const QString& title, InputType type,
                       const QString& firstLabelText, const QString& secondLabelText = QString());

  QString firstText() const;
  QString secondText() const;

 private:
  QLineEdit* firstLine_;
  QLineEdit* secondLine_;
};

}  // namespace fastonosql
