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


for i=1,10 do
    test(i)
end
--collectgarbage()

local test = CTest:new()
test:setId(777)
test:setNameAsyn("你好", function(name)
    print("asyn = "..name)
end)