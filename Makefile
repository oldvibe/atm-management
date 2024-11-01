objects = src/main.o src/system.o src/auth.o


atm : $(objects)
	cc -o atm $(objects)

clean :
	rm -f $(objects)
	rm -rf atm
