#include <string>
#include <iostream>

#include "base/mongoose.h"
#include "base/CsvParser.h"

#include "core/MgMgr.h"
#include "core/FirstController.h"

static const char *s_http_port = "8080";



int main(void)
{

    MgMgr mgr;

    FirstController* controller = new FirstController();
    controller->Init();
    
    mgr.Init(s_http_port);
    mgr.AddController("wechat", controller);
    for (;;) {
        mgr.Update(100);
    }
    mgr.UnInit();

    return 0;
}