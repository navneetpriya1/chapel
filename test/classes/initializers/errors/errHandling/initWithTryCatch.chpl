// Verifies we currently don't allow initializers to have try statements in
// their body.
// We hope to eventually allow it.
class Foo {
  var x: int;

  proc init() {
    try {
      outerFunc();
      x = 10;
    } catch {
      writeln("Look ma, I caught an error!");
    }
    super.init();
  }
}

proc outerFunc() throws {
  throw new Error();
}

var foo = new Foo();
writeln(foo);
delete foo;
