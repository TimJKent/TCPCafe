Name = "Add"
Category = "Math"
Version = "0.0.1"
Description = "Adds numbers together."
Author = "Timber Kent"


function Start()
    AddInputPin("A", PinType.Number)
    AddInputPin("B", PinType.Number)

    AddOutputPin("Sum", PinType.Number)
end

function Update()

    local a = GetPinValue("A")
    local b = GetPinValue("B")
    SetPinValue("Sum", a + b)
end