type state = {todos: array(Todo.t)};

type action =
  | NewTodo(string)
  | TodoCompleted(int)
  | RemoveTodo(int);

type filter =
  | All
  | Active
  | Completed;

let reducer = (state, action) => {
  switch (action) {
  | NewTodo(text) => {
      todos: state.todos->Belt.Array.concat([|Todo.make(~text)|]),
    }
  | _ => state
  };
};

module Counter = {
  [@react.component]
  let make = (~todos) => {
    <span>
      <strong> {todos->Belt.Array.length->React.int} </strong>
      {React.string("items left")}
    </span>;
  };
};

[@react.component]
let make = () => {
  let (state, dispatch) = React.useReducer(reducer, {todos: [||]});
  let url = ReasonReactRouter.useUrl();
  let filter =
    switch (url.hash) {
    | "active" => Active
    | "completed" => Completed
    | _ => All
    };
  <>
    <header>
      <h1> {React.string("TodoMVC")} </h1>
      <form
        onSubmit={event => {
          event->ReactEvent.Form.preventDefault;
          let text = event->ReactEvent.Form.target##newTodo##value;

          if (text->String.length > 0) {
            dispatch(NewTodo(text));
            event->ReactEvent.Form.target##reset();
          };
        }}>
        <input name="newTodo" />
      </form>
    </header>
    <main>
      <ul>
        {state.todos
         ->Belt.Array.keep(todo =>
             switch (filter, todo.completed) {
             | (Completed, true) => true
             | (Active, false) => true
             | (All, _) => true
             | _ => false
             }
           )
         ->Belt.Array.map(todo =>
             <li key={string_of_int(todo.id)}>
               {React.string(todo.text)}
             </li>
           )
         ->React.array}
      </ul>
    </main>
    <footer>
      <Counter todos={state.todos->Belt.Array.keep(todo => !todo.completed)} />
      <ul>
        <li> <a href="#"> {React.string("All")} </a> </li>
        <li> <a href="#active"> {React.string("Active")} </a> </li>
        <li> <a href="#completed"> {React.string("Completed")} </a> </li>
      </ul>
    </footer>
  </>;
};
