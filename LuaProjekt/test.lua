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
	if(timeSinceLastShot > timeSinceLastShot*corners)
		return false;
	end
	direction = {};
	speed = 1000/corners;
	size = math.sqrt(corners)*5;
	direction["x"] = playerPos["x"] - pos["x"];
	direction["y"] = playerPos["y"] - pos["y"];
	normalize(direction);
	x = direction["x"]*speed;
	y = direction["y"]*speed;
	return true, x, y, damage, size;
end
function movement( pos, playerPos )
	vector = {};
	speed = 200;
	vector["x"] = playerPos["x"] - pos["x"];
	vector["y"] = playerPos["y"] - pos["y"];
	normalize(vector);
	x = vector["x"]*speed;
	y = vector["y"]*speed;
	return y, x;
end
