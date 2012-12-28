echo ""
echo "(( encode ))"
echo ""
./f3 --encode img/300x300.bmp enc.fhb
echo ""
echo "(( decode ))"
echo ""
./f3 --decode enc.fhb decoded.bmp
