function length(vec)
	result = vec["x"]^2 + vec["y"]^2;
	return math.sqrt(result);
end
function normalize(vec)
	l = length(vec);
	vec["x"] = vec["x"] / l;
	vec["y"] = vec["y"] / l;
end
function rangedAttack( pos, playerPos, timeSinceLastShot )
	return 0;
end
function movement( pos, playerPos )
	vector = {};
	vector["x"] = pos["x"] - playerPos["x"];
	vector["y"] = pos["y"] - playerPos["y"];
	normalize(vector);
	x = -vector["x"]*200;
	y = -vector["y"]*200;
	return y, x;
end
