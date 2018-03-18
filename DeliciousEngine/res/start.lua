function thing()
	Console.Print("Hello from lua!! " .. 5.12 + 12.3)
end

function print_add(x, y)
	Console.Print(x + y)
end

print(257)

some_table = {5, 3, 3}

myvec = vec3.new(2.0, 3.0, 4.0)
print(some_table)
print(myvec)
some_vec = myvec + vec3.new(1.0)
print(some_vec)