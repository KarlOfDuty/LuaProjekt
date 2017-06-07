function update(attacking,dt)
    if not attacking then
        meleeAttack = false;
        rangedAttack = false;
        movementVector = {x=0, y=0};
        movementVector["x"], movementVector["y"], meleeAttack, rangedAttack = getInput();
        if movementVector["x"] ~= 0 or movementVector["y"] ~= 0 then
            normalize(movementVector);
            movementVector["x"] = movementVector["x"]*500*dt;
            movementVector["y"] = movementVector["y"]*500*dt;
            playerMove(movementVector["y"],movementVector["x"]);
        end
        if meleeAttack then
            print("space")
        end
        if rangedAttack then
            print("ctrl")
        end
    else

    end
end

function movement(dir,dt)
	normalize(dir);
	movementSpeed = 500;
	x = dir["x"] * movementSpeed*dt;
	y = dir["y"] * movementSpeed*dt;
	playerMove(y,x);
end

function length(vec)
	result = vec["x"]^2 + vec["y"]^2;
	return math.sqrt(result);
end

function normalize(vec)
	l = length(vec);
	vec["x"] = vec["x"] / l;
	vec["y"] = vec["y"] / l;
end
