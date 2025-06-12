Name = "PI"
Category = "Math"
Version = "0.0.1"
Description = "Return the value of pi."
Author = "Timber Kent"


function Start()
    AddOutputPin("PI", PinType.Number)
end

function Update()

    SetPinValue("PI", math.pi)
end