type t = {
  id: int,
  text: string,
  completed: bool,
};

let id = ref(0);

let make = (~text) => {
  let t = {id: id^, text, completed: false};
  id := id^ + 1;
  t;
};
