/*
 * 鼠标键盘模拟类：
 * 鼠标键盘事件的模拟大概有三种：
 * 1、通过winio 来模拟， 这个手段比较古老，但是据说有些游戏能屏蔽,而且容易发生错误
 * 2、通过windows的系统api mouse_event、 keybd_event 来操控键盘鼠标。 把游戏窗口的坐标转换位系统屏幕坐标来操作。原则上来说游戏没有权利屏蔽全局的系统api
 * 3、通过windows系统的api SendMessage给窗口发送模拟的事件消息。  这种方式最简单，但是基本上在游戏里使用不了。
 *
 * sendInput 算是keybd_event的高级封装
 */

#ifndef GAMELUA_KeyMouseMgr_H
#define GAMELUA_KeyMouseMgr_H

#include "Windows.h"



namespace gm{

    /**
     * 单例对象
     */
    class KeyMouseMgr {
    private:
        KeyMouseMgr();
    public:
        KeyMouseMgr(const  KeyMouseMgr& ) = delete;
        KeyMouseMgr& operator=(const KeyMouseMgr) =delete;

        KeyMouseMgr(KeyMouseMgr&&) =delete;
        KeyMouseMgr* operator=(KeyMouseMgr**) =delete;

        ~KeyMouseMgr();
        static KeyMouseMgr& instance(){
            static KeyMouseMgr _instance ;  // C++11里 类中的静态方法内的局部静态变量可以保证原子性和线程安全。
            return _instance;
        }


        void keyDown(int key);    // 这是mouse_event 写法
        void keyUp(int key);      // 这是mouse_event 写法
        void keyLongPress(int key,long sec);
        void MouseMoveToPoint(int x, int y);
        void MouseMoveToPointClick(int,int);
    };


}
#define sKeyMouseMgr gm::KeyMouseMgr::instance()
#endif //GAMELUA_KeyMouseMgr_H
