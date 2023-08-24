//
// Created by c0518535x on 2023/8/8.
//

#include "LuaRunner.h"





void LuaRunner::doBind() {

    for(auto [key,value] :VIRTUAL_KEY.asKeyValueRange()){
        luabridge::setGlobal(_L,value,key.toStdString().c_str());
    }

    luabridge::setGlobal(_L,_hwnd,"hwnd");


    luabridge::getGlobalNamespace(_L)
            .beginNamespace("wm")
                .beginClass<POINT>("POINT")
                        .addProperty("x",std::function<LONG(const POINT*)>([](const POINT* p){ return p->x;}))
                        .addProperty("y",std::function<LONG(const POINT*)>([](const POINT* p){ return p->y;}))
                .endClass()
                .beginClass<LuaControl>("ctrl")
                        .addConstructor<void(*)(uint)>()
                        .addFunction("sleep",&LuaControl::sleep)
                        .addFunction("msleep",&LuaControl::msleep)
                        .addFunction("keyDown",&LuaControl::keyDown)
                        .addFunction("keyUp",&LuaControl::keyUp)
                        .addFunction("keyLongPress",&LuaControl::keyLongPress)
                        .addFunction("mouseMoveToPoint",&LuaControl::mouseMoveToPoint)
                        .addFunction("mouseMoveToPointClick",&LuaControl::mouseMoveToPointClick)
                        .addFunction("findPicPos",&LuaControl::findPicPos)
                        .addFunction("findPicPosScreen",&LuaControl::findPicPosScreen)
                .endClass()
            .endNamespace();
}


LuaRunner::LuaRunner(uint hwnd,QString szFile, QObject *parent): QThread(parent) {
    _L = luaL_newstate();
    _hwnd = hwnd;
    luaL_openlibs(_L); //载入lua基础库

    doBind();
    luaL_dofile(_L,szFile.toLocal8Bit().data());
}

LuaRunner::~LuaRunner(){
    lua_close(_L);

    // 清理远程注入相关
    if(NULL != _lpPathAddr)
        VirtualFreeEx(_hProcess, _lpPathAddr, 0, MEM_RELEASE);
    if(NULL != _hThread)
        CloseHandle(_hThread);
    if(NULL != _hProcess)
        CloseHandle(_hProcess);
    if(NULL != _hToken)
        CloseHandle(_hToken);
    qDebug() << "LuaRunner Thread Destoried!";
}

void LuaRunner::runLua() {
    lua_getglobal(_L,"run");  //查找函数名
    lua_call(_L,0,0);  //没有参数，没有返回值
}

void LuaRunner::run() {

    runLua();
    //InjectWindowProcess((HWND)_hwnd);
}

// 这个是提升当前进程的权限，而不是提升目标注入进程的权限
BOOL LuaRunner::EnableDebugPrivilege() {
    BOOL fOk=FALSE;
    if(OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES,&_hToken))
    {
        TOKEN_PRIVILEGES tp;
        tp.PrivilegeCount=1;
        LookupPrivilegeValue(NULL,SE_DEBUG_NAME,&tp.Privileges[0].Luid);
        tp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;
        AdjustTokenPrivileges(_hToken,FALSE,&tp,sizeof(tp),NULL,NULL);
        fOk=(GetLastError()==ERROR_SUCCESS);
    }
    return fOk;
}

void LuaRunner::InjectWindowProcess(HWND hnd){

    auto pszDllFileName = QCoreApplication::applicationDirPath() + "/gg.dll";
    QFileInfo file(pszDllFileName);
    if(!file.isFile()){
        MessageBox(NULL,L"gg.dll 文件不存在！",L"错误",MB_OK);
        return;
    }

    // 先提升权限
    BOOL bSuccess = EnableDebugPrivilege();
    if(!bSuccess){
        MessageBox(NULL,L"提升权限失败，请尝试使用管理眼权限打开程序！",L"错误",MB_OK);
        return;
    }

    char loadfunc[25] = "LoadLibraryA";
    FARPROC loadfuncaddr = NULL;

    // 1. 打开目标进程
    DWORD pid = 0;
    GetWindowThreadProcessId(hnd,&pid);   // 获取窗口线程的进程
    _hProcess = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE,FALSE,pid);
    if(_hProcess == NULL){
        MessageBox(NULL,L"打开目标进程失败！",L"错误",MB_OK);
        return ;
    }
    qDebug() <<"hprocess base: " <<  QString::number((UINT)_hProcess,16);


    //2.在目标进程中申请空间
    _lpPathAddr = VirtualAllocEx(
            _hProcess,                                // 目标进程句柄
            0,                              // 指定申请地址
            pszDllFileName.size() + 1,   // 申请空间大小
            MEM_RESERVE | MEM_COMMIT,  // 内存的状态
            PAGE_READWRITE);                 // 内存属性
    if (NULL == _lpPathAddr)
    {
        MessageBox(NULL, L"在目标进程中申请空间失败", L"错误", MB_OK);
        CloseHandle(_hProcess);
        return;
    }
    qDebug() << "lp path addr :"  << QString::number((unsigned long long)_lpPathAddr,16);
    // 3.在目标进程中写入Dll路径
    SIZE_T dwWriteSize = 0;
    if (FALSE == WriteProcessMemory(
             _hProcess,                   // 目标进程句柄
            _lpPathAddr,                 // 目标进程地址
            pszDllFileName.toStdString().c_str(),                 // 写入的缓冲区
             pszDllFileName.size() + 1,   // 缓冲区大小
            &dwWriteSize))              // 实际写入大小
    {
        MessageBox(NULL, L"目标进程中写入Dll路径失败！", L"错误", MB_OK);
        CloseHandle(_hProcess);
        return ;
    }
    //获取LoadLibraryA的函数地址 , FARPROC可以自适应32位与64位
    FARPROC pFuncProcAddr = GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryA");
    if (NULL == pFuncProcAddr)
    {
        MessageBox(NULL, L"获取LoadLibrary函数地址失败！", L"错误", MB_OK);
        CloseHandle(_hProcess);
        return ;
    }


    // 4.在目标进程中创建线程
     _hThread = CreateRemoteThread(
             _hProcess,                   // 目标进程句柄
            NULL,                       // 安全属性
            NULL,                       // 栈大小
            (PTHREAD_START_ROUTINE)pFuncProcAddr,   // 回调函数
            _lpPathAddr,                 // 回调函数参数
            NULL,                       // 标志
            NULL                        // 线程ID
    );
    if (NULL == _hThread)
    {
        MessageBox(NULL, L"目标进程中创建线程失败！", L"错误", MB_OK);
        CloseHandle(_hProcess);
        return ;
    }
    qDebug() << "thread addr: " << QString::number((unsigned long)_hThread,16);

    // 5.等待线程结束
    WaitForSingleObject(_hThread, -1);

    // 6.清理环境
}
