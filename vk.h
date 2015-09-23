#ifndef VK_H
#define VK_H

#include <QQmlComponent>

#include "vksession.h"
#include "vkrequest.h"

namespace Vk {

    void RegisterQmlTypes () {
        qmlRegisterType<Session>("Vk", 1, 0, "VkSession");
        qmlRegisterType<Request>("Vk", 1, 0, "VkRequest");
    }

}

#endif // VK_H
