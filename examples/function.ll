// test a function called foo
block foo {
    nlecho "Now in function";
}

// entry point
start {
    nlecho "Going in to function..";
    call foo;
}


