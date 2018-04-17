Class = {
    name = "",
    id = 0,
    sum = 0
}

function Class:new(o)
    o = o or {}
    setmetatable(o, self)
    self.__index = self
    return o
end

function Class:__tostring()
    return "name = "..self.name..", id = "..self.id
end

function Class:__add(b)
    self.sum = self.sum + b.sum
    return self
end

obj = Class:new()
obj.name = "hh"
obj.id = 12
obj.sum = 1

print(obj)

function test(i)
    local ctest = CTest.new()
    ctest:setName("CTest赞")
    ctest:setId(i)
    print(ctest:getName().." "..ctest:getId())
end

-- for i=1,10 do
--     test(i)
-- end
--collectgarbage()

-- local test = CTest:new()
-- test:setId(777)
-- test:setName("meme")
-- print("subobj test")
-- test:getA():setA(111)
-- print("test:getA():getA() = "..test:getA():getA())
-- print(test:getName())

-- local test2 = CTest:new("test2")
-- print("test2 "..test2:getId().." "..test2:getName());

-- local test3 = CTest.new("test3")
-- print("test3 "..test3:getId().." "..test3:getName());

-- local bct = BClass.new()
-- bct:setB(777)
-- print("bct id = "..bct:getB())
-- test3:setB(bct)
-- print("test.B.getB = "..test3:getB():getB())

co = coroutine.create( function(a, b, c)
    print('co', a, b, c)
end )

coroutine.resume(co, 1, 2, 3);
coroutine.resume(co, 4, 5, 6);

print("end")