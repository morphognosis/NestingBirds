javac -classpath "../lib/*" -d . ../src/main/java/morphognosis/nestingbirds/*.java
copy ..\res\images\male.gif morphognosis\nestingbirds
copy ..\res\images\female.gif morphognosis\nestingbirds
copy ..\res\images\egg.gif morphognosis\nestingbirds
copy ..\res\images\cactus.gif morphognosis\nestingbirds
copy ..\res\images\tree.gif morphognosis\nestingbirds
copy ..\res\images\stone.gif morphognosis\nestingbirds
copy ..\res\images\mouse.gif morphognosis\nestingbirds
jar cvfm ../bin/nestingbirds.jar nestingbirds.mf morphognosis