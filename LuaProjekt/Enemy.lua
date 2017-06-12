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
	--Get some variables from this enemy
	pos = getPos();
	playerPos = getPlayerPos();
	corners = getCorners();
	timeSinceLastShot = getTimeSinceLastShot();
	--Move the enemy towards the player
	move(pos,playerPos,corners,dt);
	--Shoots at the player
	attack(pos,playerPos, timeSinceLastShot, corners, dt);
	--Moves this enemy out of static objects
	worldCollision();
	--Removes projectiles that hit something and returns number of times this enemy hits the player
	hits = projectilesCollision();
	if hits > 0 then
		damage = hits * (corners-2);
		--Damages the player for each projectile that hits it
		damagePlayer(damage);
	end
end

function attack(pos, playerPos, timeSinceLastShot, corners, dt)
	--If the attack is still on cooldown, don't fire
	if timeSinceLastShot > 0.4*corners then
		direction = {};
		--Decrease speed for larger enemies
		speed = 50000/corners;
		--Increase size for enemies with more corners
		size = (corners^2)*1.1;
		--Calculate the direction to fire
		direction["x"] = playerPos["x"] - pos["x"];
		direction["y"] = playerPos["y"] - pos["y"];
		normalize(direction);
		--Multiply by the speed
		x = direction["x"]*speed*dt;
		y = direction["y"]*speed*dt;
		--Returns the projectile
		createProjectile(size, y, x);
	end
end

function move(pos, playerPos, corners, dt)
	vector = {};
	--Base movement speed
	speed = 500;
	--Get a normalized vector toward the player
	vector["x"] = playerPos["x"] - pos["x"];
	vector["y"] = playerPos["y"] - pos["y"];
	normalize(vector);
	--Move the enemy in a distance depending on how many corners it has, the base speed and the framerate
	x = vector["x"]*(speed/corners)*dt;
	y = vector["y"]*(speed/corners)*dt;
	moveEnemy(y, x);
end