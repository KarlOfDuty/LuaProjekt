function length(vec)
	result = vec["x"]^2 + vec["y"]^2;
	return math.sqrt(result);
end
function normalize(vec)
	l = length(vec);
	vec["x"] = vec["x"] / l;
	vec["y"] = vec["y"] / l;
end
--Ranged attack for AI
function rangedAttackAI( pos, playerPos, timeSinceLastShot, corners )
	--If the attack is still on cooldown, don't fire
	if timeSinceLastShot < 0.4*corners then
		return 0;
	end
	direction = {};
	--Decrease speed for larger enemies
	speed = 50000/corners;
	--Increase size for enemies with more corners
	size = (corners^2)*1.1;
	--Increase damage depending on size
	damage = corners-2;
	--Calculate the direction to fire
	direction["x"] = playerPos["x"] - pos["x"];
	direction["y"] = playerPos["y"] - pos["y"];
	normalize(direction);
	--Multiply by the speed
	x = direction["x"]*speed;
	y = direction["y"]*speed;
	--Returns the projectile
	return size, damage, y, x, 1;
end
--Ranged attack for player
function shoot(direction,timeSinceLastShot)
	if timeSinceLastShot < 0.5 then
		return 0;
	end
	speed = 50000;
	size = 15;
	damage = 5;
	normalize(direction);
	x = direction["x"] * speed;
	y = direction["y"] * speed;
	return size, damage, y, x, 1;
end
--Enemies always move towards the player
function enemyMovement( pos, playerPos, size, dt )
	vector = {};
	speed = 500;
	vector["x"] = playerPos["x"] - pos["x"];
	vector["y"] = playerPos["y"] - pos["y"];
	normalize(vector);
	x = vector["x"]*(speed/size)*dt;
	y = vector["y"]*(speed/size)*dt;
	enemyMove(y, x);
end

function playerMovement(dir,dt)
	normalize(dir);
	movementSpeed = 500;
	x = dir["x"] * movementSpeed*dt;
	y = dir["y"] * movementSpeed*dt;
	playerMove(y,x);
end

function collisionWithWorld()
	worldCollision();
end

function allProjectilesCollision()
	projectileCollision();
end
