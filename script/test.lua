

local ctrl = wm.ctrl(hwnd)

function run()

--     p = ctrl:findPicsPosScreen('./data/t2.png');
--     print(p.x);
--     ctrl:mouseMoveToPointClick(p.x,p.y);

    ctrl:keyDown(VK_W)
    ctrl:sleep(5)
    ctrl:keyUp(VK_W)
end