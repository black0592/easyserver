function main()
    say("背包不够用了吗？把用不着的东西存放在仓库里吧。" ..
        "<打开仓库/openSafe/icon1>")
end

-- 打开仓库
function openSafe()
    openGUI("safe")
    close();
end

