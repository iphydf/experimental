/* SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright © 2020 The TokTok team.
 */
#include "experimental/users/iphydf/qt-toxcore-c/core.h"

#include <tox/tox.h>

namespace tox {

class ToxDeleter {
 public:
  void operator()(Tox *tox) { tox_kill(tox); }
};

struct Core::Data {
  std::unique_ptr<Tox, ToxDeleter> tox;
};

Core::Core() : data_(std::make_unique<Data>()) {
  data_->tox.reset(tox_new(nullptr, nullptr));
}

Core::~Core() {}

void Core::hello() {}

}  // namespace tox
