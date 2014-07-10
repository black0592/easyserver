
function foo (a)
    print("foo", a)  -- foo 2
    return coroutine.yield(2 * a) -- return: a , b 
end

 
--[[
co = coroutine.create(function (a , b)
    print("co-body", a, b) -- co-body 1 10
    local r = foo(a + 1)

    print("co-body2", r)
    local r, s = coroutine.yield(a + b, a - b)

    print("co-body3", r, s)

    return b, "end"
end)

        

print("main", coroutine.resume(co, 1, 10)) -- true, 4
print("------")

print("main", coroutine.resume(co, "r")) -- true 11 -9
print("------")

print("main", coroutine.resume(co, "x", "y")) -- true 10 end
print("------")

print("main", coroutine.resume(co, "x", "y")) -- false cannot resume dead coroutine
print("------")
]]



--[[
co = coroutine.create(
    function (a , b)
        print("params", a, b)
        return coroutine.yield(3, 3)
    end
)

coroutine.resume(co, 1, 2);
print(coroutine.resume(co, 4, 5))
]]


function main_test()
	for i = 1, 1000000000 do
		print("aaaa")
		if i == 5 then
			coroutine.yield()
		end
	end
end

function main_test2()
	for i = 1, 1000000000 do
		print("aaaa")
		if i == 5 then
			return
		end
	end
end

co = coroutine.create(
	function ()  
		main_test()
	end
)

coroutine.resume(co);