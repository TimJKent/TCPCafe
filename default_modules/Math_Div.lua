Name = "Divide"
Category = "Math"
Version = "0.0.1"
Description = "Divides A by B."
Author = "Timber Kent"


function Start()
    AddInputPin("A", PinType.Number)
    AddInputPin("B", PinType.Number)

    AddOutputPin("Div", PinType.Number)
end

function Update()

    local a = GetPinValue("A")
    local b = GetPinValue("B")
    SetPinValue("Div", a / b)
end