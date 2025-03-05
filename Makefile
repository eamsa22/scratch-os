CFLAGS = -Wall

TARGET1 = install # Nom de l'installeur ScratchOS
OBJFILES1 = main.o couche1.o couche2.o couche3.o couche4.o couche5.o commandes.o sha256_utils.o sha256.o

TARGET2 = scratch # Nom de l'executable ScratchOS
OBJFILES2 = scratch.o couche1.o couche2.o couche3.o couche4.o couche5.o commandes.o sha256_utils.o sha256.o

all : $(TARGET1)

$(TARGET1): $(OBJFILES1)
	gcc $(CFLAGS) -o $(TARGET1) $(OBJFILES1)

$(TARGET2): $(OBJFILES2)
	gcc $(CFLAGS) -o $(TARGET2) $(OBJFILES2)

main.o: main.c couche1.c couche1.h couche2.c couche2.h couche3.c couche3.h couche4.c couche4.h sos_defines.h

scratch.o: scratch.c couche1.c couche1.h couche2.c couche2.h couche3.c couche3.h couche4.c couche5.c couche5.h couche4.h commandes.c commandes.h sos_defines.h

couche1.o: couche1.c couche1.h couche2.c couche2.h couche3.c couche3.h couche5.c couche5.h sos_defines.h

couche2.o: couche1.c couche1.h couche2.c couche2.h couche3.c couche3.h sos_defines.h timestamp.c timestamp.h

couche3.o: couche1.c couche1.h couche2.c couche2.h couche3.c couche3.h sos_defines.h

couche4.o: couche1.c couche1.h couche2.c couche2.h couche4.c couche4.h sos_defines.h

couche5.o: couche5.c couche5.h couche1.c couche1.h couche2.c couche2.h couche3.c couche3.h timestamp.c timestamp.h sos_defines.h

commandes.o: commandes.c commandes.h couche2.c couche2.h couche3.c couche3.h couche4.c couche4.h couche5.c couche5.h sos_defines.h

timestamp.o: timestamp.c timestamp.h sos_defines.h

test_sha.o: test_sha.c sha256_utils.h

sha256_utils.o: sha256_utils.c sha256_utils.h

sha256.o: sha256.c sha256.h

clean:
	rm -rf $(TARGET1) *.o
