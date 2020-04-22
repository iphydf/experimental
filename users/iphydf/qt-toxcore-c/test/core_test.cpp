/* SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright © 2020 The TokTok team.
 */
#include "experimental/users/iphydf/qt-toxcore-c/core.h"

#include <QtTest/QtTest>

namespace tox {

class TestCore : public QObject {
  Q_OBJECT
 private slots:
  void constructor();
};

void TestCore::constructor() {
  Core core;
  core.hello();
}

}  // namespace tox

QTEST_GUILESS_MAIN(tox::TestCore)
#include "core_test.moc"
