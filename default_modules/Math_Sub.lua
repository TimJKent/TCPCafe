Name = "Subtract"
Category = "Math"
Version = "0.0.1"
Description = "Subtracts numbers."
Author = "Timber Kent"

function Start()
    AddInputPin("A", PinType.Number)
    AddInputPin("B", PinType.Number)

    AddOutputPin("Dif", PinType.Number)
end

function Update()

    local a = GetPinValue("A")
    local b = GetPinValue("B")
    SetPinValue("Dif", a - b)
end