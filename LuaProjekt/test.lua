function length(vec)
	result = vec["x"]^2 + vec["y"]^2;
	return math.sqrt(result);
end
function normalize(vec)
	l = length(vec);
	vec["x"] = vec["x"] / l;
	vec["y"] = vec["y"] / l;
end
function rangedAttack( pos, playerPos, timeSinceLastShot, corners )
	if timeSinceLastShot < 0.4*corners then
		return 0;
	end
	direction = {};
	speed = 100000/corners;
	size = corners^2;
	direction["x"] = playerPos["x"] - pos["x"];
	direction["y"] = playerPos["y"] - pos["y"];
	normalize(direction);
	x = direction["x"]*speed;
	y = direction["y"]*speed;
	return size, damage, y, x, 1;
end
function movement( pos, playerPos, size )
	vector = {};
	speed = 500;
	vector["x"] = playerPos["x"] - pos["x"];
	vector["y"] = playerPos["y"] - pos["y"];
	normalize(vector);
	x = vector["x"]*speed/size;
	y = vector["y"]*speed/size;
	return y, x;
end
