
-- Manager

function Manager:init()
    self.registered_elements = {}
end


function Manager:print_reg()
    print("start")
        for k, v in pairs(self.registered_elements) do print( k ) end
    print("end")
end

function Manager:register( name, generator )

    self.registered_elements[name] = generator
end