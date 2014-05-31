SRCDIR=$(shell pwd)/src/
DISTDIR=$(shell pwd)/dist/

all:
	-mkdir dist
	javac -version
	cd src/JSON-java/ && javac -d ${DISTDIR} -Xlint:unchecked `find . -name "*.java"`
	cd src/net/roseandsigil/colors/ && javac -d ${DISTDIR} -Xlint:unchecked `find . -name "*.java"`
	scalac-2.9 -d dist ${SRCDIR}/net/roseandsigil/colors/ColorSpaceConverter.java src/net/roseandsigil/colors/*.scala

clean:
	-rm -rf dist

run:
	cd dist && scala-2.9 net.roseandsigil.colors.Palette
