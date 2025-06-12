Name = "Square Root"
Category = "Math"
Version = "0.0.1"
Description = "Returns the square root of a number."
Author = "Timber Kent"

function Start()
    AddInputPin("A", PinType.Number)
    AddOutputPin("Sqrt", PinType.Number)
end

function Update()

    local a = GetPinValue("A")
    SetPinValue("Sqrt", math.sqrt(a))
end