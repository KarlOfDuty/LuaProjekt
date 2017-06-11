timeSinceLastShot = 0;
function length(vec)
	result = vec["x"]^2 + vec["y"]^2;
	return math.sqrt(result);
end
function normalize(vec)
	l = length(vec);
	vec["x"] = vec["x"] / l;
	vec["y"] = vec["y"] / l;
end
function update(dt)
	timeSinceLastShot = timeSinceLastShot + dt;
	pos = getPos();
	playerPos = getPlayerPos();
	corners = getCorners();
	move(pos,playerPos,corners,dt);
	--attack(pos,playerPos,timeSinceLastShot,corners);
	worldCollision();
	projectileCollision();
end

function attack(pos, playerPos, timeSinceLastShot, corners)
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
	timeSinceLastShot = 0;
	return size, damage, y, x, 1;
end

function move(pos, playerPos, corners, dt)
	vector = {};
	speed = 500;
	vector["x"] = playerPos["x"] - pos["x"];
	vector["y"] = playerPos["y"] - pos["y"];
	normalize(vector);
	x = vector["x"]*(speed/corners)*dt;
	y = vector["y"]*(speed/corners)*dt;
	moveEnemy(y, x);
end