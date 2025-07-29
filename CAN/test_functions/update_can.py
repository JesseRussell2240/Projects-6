from fetch_params import fetch_motor_parameters

params = fetch_motor_parameters()

for name, value in params.items():
	print(f"{name}: {value}")

