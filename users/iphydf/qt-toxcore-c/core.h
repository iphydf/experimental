/* SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright © 2020 The TokTok team.
 */
#ifndef EXPERIMENTAL_USERS_IPHYDF_QT_TOXCORE_C_CORE_H
#define EXPERIMENTAL_USERS_IPHYDF_QT_TOXCORE_C_CORE_H

#include <QObject>
#include <memory>

namespace tox {

class Core : QObject {
  Q_OBJECT

 public:
  Core();
  ~Core();

  void hello();

 private:
  struct Data;
  std::unique_ptr<Data> data_;
};

}  // namespace tox

#endif  // EXPERIMENTAL_USERS_IPHYDF_QT_TOXCORE_C_CORE_H
