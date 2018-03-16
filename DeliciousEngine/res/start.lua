function thing()
	Console.Print("Hello from lua!! " .. 5.12 + 12.3)
end

function print_add(x, y)
	Console.Print(x + y)
end
Console.RegisterCommand("print_add")

thing()