Name = "Print"
Category = "Output"
Version = "0.0.1"
Description = "Prints provided string to console."
Author = "Timber Kent"

local inc = 0

function Start()
    inc = inc +1
    AddInputPin("Str", PinType.Any)
    AddInputPin("Trigger", PinType.Bool)
end

function Update()
    if GetPinValue("Trigger") then
        print(GetPinValue("Str"))
    end
end