Name = "Multiply"
Category = "Math"
Version = "0.0.1"
Description = "Multiplies numbers together."
Author = "Timber Kent"


function Start()
    AddInputPin("A", PinType.Number)
    AddInputPin("B", PinType.Number)

    AddOutputPin("Prod", PinType.Number)
end

function Update()

    local a = GetPinValue("A")
    local b = GetPinValue("B")
    SetPinValue("Prod", a * b)
end