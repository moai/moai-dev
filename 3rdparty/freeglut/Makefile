?FLASCC:=/path/to/FLASCC/SDK
$?GLS3D:=/path/to/gls3d

SRC=$(wildcard src/*.c)
OBJS=$(subst .c,.o,$(SRC))

AS3COMP=java -jar $(FLASCC)/usr/lib/asc2.jar -merge -md -AS3 -optimize \
	-import $(FLASCC)/usr/lib/builtin.abc \
	-import $(FLASCC)/usr/lib/playerglobal.abc \
	-import $(FLASCC)/usr/lib/BinaryData.abc \
	-import $(FLASCC)/usr/lib/ISpecialFile.abc \
	-import $(FLASCC)/usr/lib/IBackingStore.abc \
	-import $(FLASCC)/usr/lib/IVFS.abc \
	-import $(FLASCC)/usr/lib/InMemoryBackingStore.abc \
	-import $(FLASCC)/usr/lib/PlayerKernel.abc \
	-import $(FLASCC)/usr/lib/CModule.abc \
	-import $(GLS3D)/install/usr/lib/libGL.abc

.PHONY: demos

all:
	@mkdir -p install/usr/lib
	
	@echo "Compiling libglut.o"
	make compile
	@$(FLASCC)/usr/bin/ar crus install/usr/lib/libglut.a src/*.o

	make demos -j6

clean:
	rm -f src/*.o demos/*.swf demos/*.abc

demos/Console.abc: demos/Console.as
	@echo "Compiling Console.abc"
	@$(AS3COMP) \
	demos/Console.as -outdir demos -out Console

lesson1: demos/Console.abc
	@echo "Compiling lesson1.swf"
	@$(FLASCC)/usr/bin/g++ -O4 -flto-api=exports.txt \
		-I$(GLS3D)/install/usr/include/ \
		-Iinstall/usr/include/ \
		-Linstall/usr/lib/ \
		-L$(GLS3D)/install/usr/lib/ \
		demos/NeHeLesson01/lesson1.cpp $(GLS3D)/install/usr/lib/libGL.abc -lGL -lglut \
		-symbol-abc=demos/Console.abc -swf-size=500x500 -emit-swf -o demos/lesson1.swf

lesson2: demos/Console.abc
	@echo "Compiling lesson2.swf"
	@$(FLASCC)/usr/bin/g++ -O4 -flto-api=exports.txt  \
		-I$(GLS3D)/install/usr/include/ \
		-Iinstall/usr/include/ \
		-Linstall/usr/lib/ \
		-L$(GLS3D)/install/usr/lib/ \
		demos/NeHeLesson02/lesson2.cpp $(GLS3D)/install/usr/lib/libGL.abc -lGL -lglut \
		-symbol-abc=demos/Console.abc -swf-size=500x500 -emit-swf -o demos/lesson2.swf

shapes: demos/Console.abc
	@echo "Compiling shapes.swf"
	@$(FLASCC)/usr/bin/gcc -O4 -flto-api=exports.txt  \
		-I$(GLS3D)/install/usr/include/ \
		-Iinstall/usr/include/ \
		-Linstall/usr/lib/ \
		-L$(GLS3D)/install/usr/lib/ \
		demos/shapes.c $(GLS3D)/install/usr/lib/libGL.abc -lGL -lglut \
		-symbol-abc=demos/Console.abc -swf-size=500x500 -emit-swf -o demos/shapes.swf

lesson19: demos/Console.abc
	@echo "Compiling lesson19.swf"
	@rm -f demos/NeHeLesson19/embed*
	@cd demos/NeHeLesson19 && $(FLASCC)/usr/bin/genfs --type=embed vfs embed
	@cd demos/NeHeLesson19/ && $(AS3COMP) \
	embed*.as -outdir . -out embed
	@$(FLASCC)/usr/bin/g++ -O4 -flto-api=exports.txt  \
		-I$(GLS3D)/install/usr/include/ \
		-Iinstall/usr/include/ \
		-Linstall/usr/lib/ \
		-L$(GLS3D)/install/usr/lib/ \
		demos/NeHeLesson19/lesson19.cpp $(GLS3D)/install/usr/lib/libGL.abc -lGL -lglut \
		-symbol-abc=demos/Console.abc demos/NeHeLesson19/embed*.abc -swf-size=500x500 -emit-swf -o demos/lesson19.swf

lesson11: demos/Console.abc
	@echo "Compiling lesson11.swf"
	@rm -f demos/NeHeLesson11/embed*
	@cd demos/NeHeLesson11 && $(FLASCC)/usr/bin/genfs --type=embed vfs embed
	@cd demos/NeHeLesson11/ && $(AS3COMP) \
	embed*.as -outdir . -out embed
	@$(FLASCC)/usr/bin/g++ -O4 -flto-api=exports.txt  \
		-I$(GLS3D)/install/usr/include/ \
		-Iinstall/usr/include/ \
		-Linstall/usr/lib/ \
		-L$(GLS3D)/install/usr/lib/ \
		demos/NeHeLesson11/lesson11.cpp $(GLS3D)/install/usr/lib/libGL.abc -lGL -lglut \
		-symbol-abc=demos/Console.abc demos/NeHeLesson11/embed*.abc -swf-size=500x500 -emit-swf -o demos/lesson11.swf

lesson12: demos/Console.abc
	@echo "Compiling lesson12.swf"
	@rm -f demos/NeHeLesson12/embed*
	@cd demos/NeHeLesson12 && $(FLASCC)/usr/bin/genfs --type=embed vfs embed
	@cd demos/NeHeLesson12/ && $(AS3COMP) \
	embed*.as -outdir . -out embed
	@$(FLASCC)/usr/bin/g++ -O4 \
		-I$(GLS3D)/install/usr/include/ \
		-Iinstall/usr/include/ \
		-Linstall/usr/lib/ \
		-L$(GLS3D)/install/usr/lib/ \
		demos/NeHeLesson12/lesson12.cpp $(GLS3D)/install/usr/lib/libGL.abc -lGL -lglut \
		-symbol-abc=demos/Console.abc demos/NeHeLesson12/embed*.abc -swf-size=500x500 -emit-swf -o demos/lesson12.swf

lesson8: demos/Console.abc
	@echo "Compiling lesson8.swf"
	@rm -f demos/NeHeLesson08/embed*
	@cd demos/NeHeLesson08 && $(FLASCC)/usr/bin/genfs --type=embed vfs embed
	@cd demos/NeHeLesson08/ && $(AS3COMP) \
	embed*.as -outdir . -out embed
	@$(FLASCC)/usr/bin/g++ -O4 -flto-api=exports.txt  \
		-I$(GLS3D)/install/usr/include/ \
		-Iinstall/usr/include/ \
		-Linstall/usr/lib/ \
		-L$(GLS3D)/install/usr/lib/ \
		-Idemos/NeHeCommon/ \
		demos/NeHeLesson08/lesson8.cpp demos/NeHeCommon/tgaload.cpp $(GLS3D)/install/usr/lib/libGL.abc -lGL -lglut \
		-symbol-abc=demos/Console.abc demos/NeHeLesson08/embed*.abc -swf-size=500x500 -emit-swf -o demos/lesson8.swf

lesson7: demos/Console.abc
	@echo "Compiling lesson7.swf"
	@rm -f demos/NeHeLesson07/embed*
	@cd demos/NeHeLesson07 && $(FLASCC)/usr/bin/genfs --type=embed vfs embed
	@cd demos/NeHeLesson07/ && $(AS3COMP) \
	embed*.as -outdir . -out embed
	@$(FLASCC)/usr/bin/g++ -O4 -flto-api=exports.txt  \
		-I$(GLS3D)/install/usr/include/ \
		-Iinstall/usr/include/ \
		-Linstall/usr/lib/ \
		-L$(GLS3D)/install/usr/lib/ \
		-Idemos/NeHeCommon/ \
		demos/NeHeLesson07/lesson7.cpp demos/NeHeCommon/tgaload.cpp $(GLS3D)/install/usr/lib/libGL.abc -lGL -lglut \
		-symbol-abc=demos/Console.abc demos/NeHeLesson07/embed*.abc -swf-size=500x500 -emit-swf -o demos/lesson7.swf

lesson6: demos/Console.abc
	@echo "Compiling lesson6.swf"
	@rm -f demos/NeHeLesson06/embed*
	@cd demos/NeHeLesson06 && $(FLASCC)/usr/bin/genfs --type=embed vfs embed
	@cd demos/NeHeLesson06/ && $(AS3COMP) \
	embed*.as -outdir . -out embed
	@$(FLASCC)/usr/bin/g++ -O4 -flto-api=exports.txt  \
		-I$(GLS3D)/install/usr/include/ \
		-Iinstall/usr/include/ \
		-Linstall/usr/lib/ \
		-L$(GLS3D)/install/usr/lib/ \
		-Idemos/NeHeCommon/ \
		demos/NeHeLesson06/lesson06.cpp demos/NeHeCommon/tgaload.cpp $(GLS3D)/install/usr/lib/libGL.abc -lGL -lglut \
		-symbol-abc=demos/Console.abc demos/NeHeLesson06/embed*.abc -swf-size=500x500 -emit-swf -o demos/lesson6.swf

lesson5: demos/Console.abc
	@echo "Compiling lesson5.swf"
	@$(FLASCC)/usr/bin/gcc -O4 -flto-api=exports.txt  \
		-I$(GLS3D)/install/usr/include/ \
		-Iinstall/usr/include/ \
		-Linstall/usr/lib/ \
		-L$(GLS3D)/install/usr/lib/ \
		demos/NeHeLesson05/lesson5.c $(GLS3D)/install/usr/lib/libGL.abc -lGL -lglut \
		-symbol-abc=demos/Console.abc -swf-size=500x500 -emit-swf -o demos/lesson5.swf

lesson3: demos/Console.abc
	@echo "Compiling lesson3.swf"
	@$(FLASCC)/usr/bin/gcc -O4 -flto-api=exports.txt  \
		-I$(GLS3D)/install/usr/include/ \
		-Iinstall/usr/include/ \
		-Linstall/usr/lib/ \
		-L$(GLS3D)/install/usr/lib/ \
		demos/NeHeLesson03/lesson3.c $(GLS3D)/install/usr/lib/libGL.abc -lGL -lglut \
		-symbol-abc=demos/Console.abc -swf-size=500x500 -emit-swf -o demos/lesson3.swf

lesson4: demos/Console.abc
	@echo "Compiling lesson4.swf"
	@$(FLASCC)/usr/bin/gcc -O4 -flto-api=exports.txt  \
		-I$(GLS3D)/install/usr/include/ \
		-Iinstall/usr/include/ \
		-Linstall/usr/lib/ \
		-L$(GLS3D)/install/usr/lib/ \
		demos/NeHeLesson04/lesson4.c $(GLS3D)/install/usr/lib/libGL.abc -lGL -lglut \
		-symbol-abc=demos/Console.abc -swf-size=500x500 -emit-swf -o demos/lesson4.swf
	@echo "Compiling shapes.swf"

demos: lesson1 lesson2 lesson3 lesson4 lesson5 lesson6 lesson7 lesson8 lesson11 lesson12 lesson19 shapes

compile: $(OBJS)

.c.o:
	@echo "Compiling $<"
	@$(FLASCC)/usr/bin/gcc -DHAVE_GETTIMEOFDAY -DHAVE_ERRNO_H -DHAVE_LIMITS_H -DHAVE_VFPRINTF -DHAVE_UNISTD_H -O4 -c \
		-I$(GLS3D)/install/usr/include/ -Iinstall/usr/include/ $< -o $@
