require "person_pb"

local msg = person_pb.Person()  
msg.id = 100   
msg.name = "foo"   
msg.email = "bar"   
  
local pb_data = msg:SerializeToString()  -- Parse Example  
print("create:", msg.id, msg.name, msg.email, pb_data)  
  
local msg = person_pb.Person()   
msg:ParseFromString(pb_data)   
print("parser:", msg.id, msg.name, msg.email, pb_data)




