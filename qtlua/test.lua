print("Hello World")

main = function()
  local w = Widget:new()
  w:setWindowTitle("你好世界")
  w:show()
end

main()

local Sub = {
  value = 0,
  index = 0
}

function Sub:new(o)
  o = o or {}
  setmetatable(o, self)
  self.__index = self
  return o;
end

function Sub:show()
  print(">>>>>>>>>>>>>>>>>>>>>>>>>")
  for key, value in pairs(self) do
    print(key.." "..value)
  end
  print("<<<<<<<<<<<<<<<<<<<<<<<<<")
end

a = Sub:new {}
a.value = 4
a:show()

print("===================")

local PSub = Sub:new {ps = 0}

function PSub:dis()
  print("ps = "..self.ps)
end

b = PSub:new()
b:dis()
b:show()

print("end")
