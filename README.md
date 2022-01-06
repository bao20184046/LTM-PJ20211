# LTM-PJ20211
## update1: Login and Register
## update2: Create room and Join room.(Some bug is constaining in create room module)
## update3: Complete game play(Some bug is costaining in show result)
### Run server:

```
gcc -o server server.c user.c protocol.c room.c card.c handcard.c
```
### Run client:

```
gcc -o client client.c protocol.c
```
### Run client in update3:
```
gcc -o client client.c protocol.c card.c
```
