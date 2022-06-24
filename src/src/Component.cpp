#pragma once

#include "../include/Component.hpp"

Component::Component() : associated(*(GameObject*)nullptr) { }
Component::Component(GameObject& associated): associated(associated) { }
Component::~Component() {}

void Component::Start() {}