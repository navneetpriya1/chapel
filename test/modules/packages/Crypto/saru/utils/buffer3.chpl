proc main() {
  use Crypto;
  var a = new CryptoBuffer(""); // doesn't do anything
  writeln(a.toHex());
}
