lastmoveVector = {x=1, y=0};
meleeAttack = false;
meleeRotation = 0;
rangedAttack = false;
timeSinceLastShot = 0;

function update(dt)
    timeSinceLastShot = timeSinceLastShot + dt;
    if not meleeAttack then
        directionVector = {x=0, y=0};
        directionVector["x"], directionVector["y"], meleeAttack, rangedAttack = getInput();
        if directionVector["x"] ~= 0 or directionVector["y"] ~= 0 then
            normalize(directionVector);
            lastmoveVector["x"] = directionVector["x"];
            lastmoveVector["y"] = directionVector["y"];
            movementVector = {x=0, y=0};
            movementVector["x"] = directionVector["x"]*500*dt;
            movementVector["y"] = directionVector["y"]*500*dt;
            playerMove(movementVector["y"],movementVector["x"]);
        end
        if meleeAttack then
            useMeleeAttack(lastmoveVector["x"],lastmoveVector["y"]);
            meleeAttack = true;
            meleeRotation = 0;
        end
        if rangedAttack  and timeSinceLastShot > 0.5 then
            xVelocity = lastmoveVector["x"]*1000;
            yVelocity = lastmoveVector["y"]*1000;
            shoot(xVelocity, yVelocity, 20);
            timeSinceLastShot = 0;
        end
    else
        meleeRotation = meleeRotation + 666*dt;
        rotateMelee(666*dt);
        meleeCollision();
        if meleeRotation > 180 then
            meleeAttack = false;
            resetMeleeCooldowns();
        end
    end
    mtv = {x=0,y=0};
    mtv["x"],mtv["y"] = playerCollision();
    playerMove(mtv["y"],mtv["x"]);
    projectilesCollision(dt);
    return meleeAttack;
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
