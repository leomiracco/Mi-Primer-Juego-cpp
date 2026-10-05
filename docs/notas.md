# Cómo funciona hasta ahora el motor (🔴🟢🟡🔵):

> [1] Cómo está pensando la arquitectura:
🟡Bueno. Vamos a tener el archivo main.cpp desde donde se inicia todo. Crea la Clase Game y comienza a ejecutar su método Init() y su método Run().
🟡Luego, vamos a tener una Clase Game que tiene constructor vacío. Va a crear la ventana de la interfaz gráfica del juego, el nombre del mismo que aparece en la barra de título. Se crean métodos públicos y privados que van a manejar todo el juego, los métodos públicos van a ser, el nombre del juego, y las dimensiones de la ventana. Luego un método donde se ejecuta el loop del juego y otro método público para limpiar todo los objetos creados al finalizar la ventana. Los métodos privados son para detectar si el usuario a ingresado algún dato, a tocado alguna tecla o botón del joystick. Luego el update es el que se encarga de realizar los cambios del juego en tiempo de ejecución, como mover objetos, personajes, etc. Y finalmente, el render que es borrar toda la pantalla y volver a pintarla con todos los objetos en sus nuevas posiciones si es que se han movido. Y, además, se declara una variable privada de tipo GameObject que ahora mismo no existe, pero lo hará en el futuro inmediato. Esta Clase es la que organiza la orquesta, es como si fuese el Controller en el patrón de diseño Vista Modelo Controlador. Es el director de una orquesta. Es decir, si en algún momento nos vemos tentados a empezar a "tocar un instrumento" (por ejemplo, escribir cálculos de salto, vidas del personaje o colores dentro de Game), debes detenerte.
🟡La idea es crear una colección o lista de GameObjects que pueden ser cualquier cosa, desde un Player, enemigo, muro, parlante, moneda de oro. Pero estos GameObjects van a tener componentes hijos como Trasforms, SquareRenderer, EnemyAI, AudioSource, CoinScript, etc. En los ejemplos anteriores de GameObjects, todos van a tener componentes hijos Transforms, luego, todos van a tener hijos SquareRenderer menos el parlante, y solo va a contener un componente hijo Hero el Player, solo va a contener un componente hijo EnemyAI el enemigo, solo va a contener el componente hijo AudioSource el parlante, y solo va a contener el hijo CoinScript la moneda de oro. Esta herencia de un GameObject con su componente hijo no está creada al nacer un objeto GameObject. Solo los componentes (Trasforms, SquareRenderer, EnemyAI, AudioSource, CoinScript, etc.) van a heredar al nacer de la Clase Component. Pero luego, en el momento en que se está creando dicho componente, se le asigna de forma manual/casera a su padre gameObject (Player, enemigo, muro, etc.). De esta manera, cada componente va a tener asociado para siempre su correspondiente gameObject. Entonces, cada componente de un gameObject se encarga de solo su tarea, en el caso del componente Transform, solo va a saber sus coordenadas y sus dimensiones, en el caso del componente SquareRenderer solo va a conocer su aspecto, que forma tiene, qué colores, y también debe conocer su posición inicial y sus dimensiones, porque es el encargado de pintarlo en el escenario, y en el caso del gameObject Player, su componente Hero solo va a conocer su comportamiento, a la velocidad que se desplaza y cuando recorra todo el escenario, que vuelva al punto de partida para volver a realizar el recorrido en un blucle infinito, todo eso es el comportamiento.
🟡Cada componente va a encargarse de una única tarea puntual, sin conocer la existencia de otro componente.
🟡Y la Clase Time.h lo que hace es aseguranos que cualquier hardware lento va a ir sincronizado en tiempo real con alguien que juegue en un Hardware potente, eso sí, a costo de ver saltos en caso de ir quedándose un poco atrás en el proceso.

> [2] Antes de recorrer todo el código o flujo de datos en tiempo de ejecución, vamos a hacer lo mismo pero siendo nosotros el compilador:
🔵Lo que hace y NO hace el Compilador:
  1) El compilador NUNCA reserva memoria para los objetos.
  2) El compilador no crea objetos.
  3) El compilador es solo un fabricante de moldes.
  4) Su trabajo es calcular --> ¿Cuánto espacio ocupará un Transform cuando el juego se abra? Ocupará 16 bytes.
  5) ¿Cuánto un puntero? 8 bytes.
  6) Anota esos tamaños en el archivo ejecutable (.exe) y nada más. La memoria real se pide recién cuando el usuario hace doble clic en el juego.
  7) Cuando entra en Component.h y se encuentra con la línea --> class GameObject; Va a existir ese nombre. Como lo que viene abajo es un puntero (GameObject*), y yo ya sé que todos los punteros miden 8 bytes, no necesito saber nada más.
  8) Cada arhivo.cpp es una «Isla Independiente». El compilador no salta de un archivo.cpp a otro. Cada archivo.cpp se compila en una habitación cerrada y aislada (se llaman Unidades de Traducción).

<El Camino Real que recorre el compilador (Paso a Paso):>

🔵Etapa 1: Compilar la Isla 1 (src/main.cpp)
El compilador abre main.cpp sin tener la menor idea de que Game.cpp existe.
  1) Lee #include "Core/Game.h".
  Pausa main.cpp y viaja a Game.h.
  2) En Game.h ve #include "ECS/GameObject.h".
  Pausa y viaja a GameObject.h.
  3) En GameObject.h ve #include "Component.h".
  Pausa y viaja a Component.h.
  4) Lee Component.h, ve class GameObject;, aprende el molde base y regresa.
  5) Termina de leer GameObject.h y regresa.
  6) Termina de leer Game.h (aprende que Game tiene funciones llamadas Init, Run y Clean) y regresa.

  Vuelve a main.cpp y lee:
  <game.Init("Mi Primer Juego C++", 800, 600);
  game.Run();>

  🔴El compilador dice:
    «En el menú de Game.h vi que existen esas funciones. No tengo idea de qué código tienen adentro porque aquí no tengo a Game.cpp, pero confío en que existen. Dejo una nota pendiente que dice: "Buscar el código de Init y Run más tarde"».

  Termina la Isla 1: Genera en el disco un archivo binario llamado <main.obj>.

🔵Etapa 2: Compilar la Isla 2 (src/Core/Game.cpp)
  1) El compilador se olvida de main.cpp y ahora abre Game.cpp desde cero.
  2) Abre Game.h, Time.h, Components/Transform.h, Components/SquareRenderer.h, Components/Hero.h
  Absorbe todas esas clases y aprende cómo se dibujan los rectángulos y cómo se mueven.
  3) Continúa con los métodos: Game::Init(), Game::Run() y Game::Clean().
  4) Termina la Isla 2: Genera en el disco otro archivo binario llamado Game.obj

🔵Etapa 3: Entra el «Enlazador» (Linker) — El paso que te faltaba  
  1) Toma main.obj y lee la nota pendiente:
  «main necesita ejecutar game.Init() y game.Run()».
  2) Busca en Game.obj y dice:
  «¡Aquí está el código binario real de esas funciones!».
  3) Pega ambos archivos juntos, agrega las librerías externas de SDL2 y fabrica el producto final:
  👉 FirstGame.exe.

🔵Resumen para fijar en tu mente:
  1) Los archivos .h: Son contratos o «menús» que se copian y pegan dentro de cada .cpp para que el compilador sepa qué funciones y clases existen.
  2) El Compilador: Cocina cada archivo .cpp por separado, produciendo archivos .obj.
  3) El Linker (Enlazador): Es el pegamento final que une todos los .obj para crear el juego ejecutable (.exe).

> [3] Explicación de la declaración de "class GameObject;" en la Clase Component.h:
Cualquiera pensaría que el compilador se confundiría al ver dos veces la palabra class GameObject.
La razón por la que no se marea en lo más mínimo es porque en C++ existe una regla sagrada que separa dos conceptos: Declarar vs. Definir.
Para entender cómo lo sabe el compilador, imaginemos la Tabla de Símbolos (que es como la libreta de notas del compilador):
# La analogía del Hotel y la Reserva
Piensa en el compilador como el recepcionista de un hotel con una libreta en la mano:

1. Cuando lee class GameObject; en Component.h:
Es como si llamaras por teléfono al hotel y dijeras:
«Hola, reservo una habitación a nombre de GameObject. Más tarde voy a llegar».
El recepcionista anota en su libreta:
[LIBRETA DEL COMPILADOR]
Nombre: GameObject
Tipo: Clase
Estado: INCOMPLETO (Sé que el nombre existe, pero todavía no vi su cara ni su equipaje).
Con solo esa anotación, cuando en la línea siguiente ve GameObject* gameObject;, el recepcionista dice:
«Ah, un puntero para el huésped GameObject. No necesito ver su equipaje todavía, solo le reservo la llave (8 bytes)».

2. Cuando regresa a GameObject.h y ve class GameObject { ... }:
Una hora más tarde, entras físicamente por la puerta del hotel:
«Hola, soy GameObject en persona, aquí tengo mi equipaje (mis variables) y mis habilidades (mis métodos)».

¿Qué hace el recepcionista? No se asusta ni piensa que es otra persona.
Mira su libreta y busca: «¿Tengo anotado a alguien llamado GameObject?».
Ve que sí: «¡Ah, sí! Eras el que tenía la ficha en estado INCOMPLETO».
Y en vez de crear una persona nueva, completa la ficha que ya tenía abierta:
[LIBRETA DEL COMPILADOR]
Nombre: GameObject
Tipo: Clase
Estado: COMPLETO ✅
Tamaño: 40 bytes
Variables: name, components
Métodos: Update, Render, AddComponent...

# ¿Cómo sabe que es el MISMO GameObject?
Lo sabe por una sola cosa: el Nombre y el Ámbito (Scope).
En C++, dentro del mismo espacio (ámbito global):
Puedes declarar (anunciar) que una clase existe todas las veces que quieras:
class GameObject; // Anuncio 1 (Ok)
class GameObject; // Anuncio 2 (Ok, ya lo sabía)
class GameObject; // Anuncio 3 (Ok, ya lo sabía)
El compilador no se queja; simplemente dice: «Sí, ya sé que existe».
Pero solo puedes definirla (mostrar las llaves { ... } con su código) UNA SOLA VEZ en todo el programa. Esto se llama la Regla de Definición Única (One Definition Rule).

# ¿Cuándo SÍ se marearía y daría error?

Se marearía si intentaras hacer esto:
// Archivo 1:
class GameObject { int vida; };

// Archivo 2:
class GameObject { float velocidad; }; // ❌ ERROR FATAL: Redefinición de clase

Ahí el compilador diría: «¡Momento! Ya me trajiste un cuerpo entero para GameObject antes, no me puedes traer otro cuerpo diferente con el mismo nombre».
# En conclusión:
class GameObject; es solo anunciar que el nombre existe (ficha abierta, tipo incompleto).
class GameObject { ... }; es traer el cuerpo real (cerrar la ficha, tipo completo).
El compilador simplemente une los dos puntos porque tienen el mismo nombre.

> [4] El flujo de datos en tiempo de ejecución. Se ejecuta el juego desde el archivo main.cpp
🟢Crea el objeto Game game. Dicha Clase tiene un constructor vacío. Luego ejecuta un método de la Clase Game que devuelve un booleando. game.Init("Mi Primer Juego C++", 800, 600).
El método Init() continúa inicializando la librería SDL. Luego crea la ventana window con SDL_CreateWindow con el nombre del título pasado como parámetro en la ejecución de dicho método Init() de la Clase Game. Luego con SDL_CreateRenderer crea el motor de dibujo (el pincel de la GPU). Le dice a tu tarjeta gráfica: «Prepárate para dibujar sobre esta ventana». La acción real de pintar la pantalla ocurrirá recién en la Fase 3, dentro del bucle en void Game::Render() de la misma Clase.
🟢Luego, Cuando llegas a Game::Init(), el estuche ya estaba sobre la mesa esperándote. El estuche vacío nace en la primera línea de main.cpp cuando escribes: Game game;. Como player es una variable miembro de Game, en el segundo en que game nace en el Stack, su estuche player nace con él <std::unique_ptr<GameObject> player = nullptr;> valiendo puntero nulo (Se crea una variable especial del tipo  tiene su propio constructor, su propio destructor y sus propios métodos (como .get()). Aunque sea una clase/objeto, su único trabajo en la vida es gestionar un puntero). Entonces en Game.cpp cuando llegamos a la línea: <player = std::make_unique<GameObject>("Player");> lo único que haces es fabricar la guitarra y meterla adentro del estuche.
Entonces en la variable/puntero de objeto <player> de tipo GameObject apuntamos a él y cuya única propiedad es su name = "Player".
Cuando llegamos en Game.cpp a la línea --> <auto* transform = player->AddComponent<Transform>();> Transform no da error porque en la cabecera tenemos --> #include "Components/Transform.h", entonces lo que hace la línea esa es ejecutar el método AddComponent de tipo T* (devuelve un puntero de tipo T = componente) de la Clase GameObject, en dicho método se pasa la referencia Transform en esta línea --> <auto newComponent = std::make_unique<T>();> almacenando en newComponent el objeto Transform. Luego, con <newComponent->gameObject = this;> se accede a la propiedad gameObject de Transform que hereda de Component y le asigna el objeto "Player" para siempre quedar ligados hasta que se cierre el juego. Luego en esta línea --> <T* rawPtr = newComponent.get();> solo se guarda la referencia/puntero del objeto Transform en rawPtr y se agrega a la lista <components.push_back(std::move(newComponent));> quedando la variable newComponent vacía (nullptr) pero la dirección de memoria ahora vive dentro del vector components. Tomando la precaución de guardar una fotocopia en rawPtr una línea antes, rawPtr->Init() sigue funcionando perfectamente. Luego se ejecuta el método <rawPtr->Init();> como Transform no tiene sobre escrito ese método, va a la Clase que hereda Component y ejecuta el método aunque éste está vacío. Por último devuelve el puntero de Transform en este caso.
Seguimos en la Clase Game.cpp y vamos asignándole los valores nuevos a las propiedades x, y, ancho y largo de la Clase Transform que cuyo valor en la propiedad gameObject es "Player".
Luego viene la línea --> <player->AddComponent<SquareRenderer>();> lo que hacemos es de nuevo ejecutar el método AddComponent de la Clase GameObject (porque player es un gameObject), que regresa un tipo T. Cuando llegamos a la línea <auto newComponent = std::make_unique<T>();> de nuevo, como ahora le pasamos por parámetro el Componente SquareRender, crea el objeto SquareRender sin constructor con el constructor vacío. Le asigna a la variable gameObject el GameObject player para quedar ligado para siempre. Siempre y cuando no se vuelva a asignar desde otro lugar del código. Se agrega a la lista de Componentes, se ejecuta el método Init() que la Clase SquareRenderer sí la sobreescribió y lo que hace es ejecutar el método GetComponent de la Clase GameObject. Entonces, empieza a recorrer la lista components. Importante, se va a crear una mochila (con sus componentes) por GameObject. Es decir, si se crean 10 GameObject, cada uno tendrá su propia lista de componentes, habrá tantas listas como GameObject existan. Entonces, como cada gameObject creado, solo cuenta con sus propios componentes, cuando se llama al método GetComponent desde en este caso el componente SquareRenderer a partir de su padre en este caso Player, solo va a buscar la mochila del GameObject de Player para encontrar su Transform y devolver su puntero. Si no encuentra nada, devuelve nullptr (puntero nulo).
Luego llegamos a la línea --> <player->AddComponent<Hero>();> de la Clase Game.cpp, donde se ejecuta el método AddComponent de la Clase GameObject (porque player es un gameObject), que regresa un tipo T. Cuando llegamos a la línea <auto newComponent = std::make_unique<T>();> de nuevo, como ahora le pasamos por parámetro el Componente Hero, crea el objeto Hero sin constructor con el constructor vacío. Le asigna a la variable gameObject el GameObject player (el puntero del GameObject player) para quedar ligado para siempre. Se agrega a la lista de Componentes, se ejecuta el método Init() que la Clase Hero sí la sobreescribió y lo que hace es ejecutar el método GetComponent de la Clase GameObject, para obtener el puntero Transform su hermano de player. Le asigna a la variable isRunning = true; para que llegado el momento el loop del juego comience. Y devuelve true.
Si devuelve true, desde main.cpp se ejecuta el método Run() de la Clase Game: game.Run();
Se comienza con el bucle while mientras isRunning sea true, luego se ejecuta el método Update() de la Clase Time: Time::Update(); que lo que hace es saber cuánto tiempo tarde en cada ciclo.
Luego se ejecuta el método HandleEvents(); de esta misma Clase Game.cpp. Lo que hace es, cuando se da clic sobre la cruz de la ventana, se le asigna a la variable isRunning = false, eso hace que se detenga el ciclo while. Lo que generaría que en main.cpp salga del condicional if y se ejecute el método Clean() de la Clase Game.cpp, este método destruye el renderer, destruye el window y termina la librería SDL.
Seguimos en el método Run() de la clase Game.cpp y ejecutamos el método Update() de esta misma Clase. Esta lo que hace es ejecutar el método Update() del GameObject player <player->Update(Time::GetDeltaTime());> pasándole como parámetro el valor de lo que tardó el ciclo del loop en ese instante. El método Update() en la Clase GameObject lo que hace es recibir el tiempo con decimales, recorrer los componentes de ese player e ir ejecutándole el método Update() a cada uno de ellos. Pero solo Hero ha sobreescrito el método Update() que es lo correcto porque lo que se quiere hacer es ir moviendo al personaje en cada ciclo del loop. Entonces, lo que hace ese método Update() en Hero es, primero asegurse que la variable transform tenga el puntero o referencia al objeto Transorm del GameObject player, si es así, en al coordenada x del transform le asigna el resultado de multiplicar speed (en este caso vale 250 px) * el tiempo que tardó el ciclo inmediatamente anterior. Luego pregunta si la coordenada x de transform es mayor a 800 px (significa que el personaje está fuera del ancho del escenario) y si es así lo traslada al punto en x de -100, si no continúa.
Luego pasamos al método Render(); de la Clase Game.cpp, lo que hace es pintar de azul oscuro con SDL_SetRenderDrawColor(renderer, 30, 35, 45, 255); 👉 Moja el rodillo gigante: Le dice a la GPU: «El color activo ahora es el azul oscuro». (Todavía no pinta nada, solo eligió la pintura). Y luego con SDL_RenderClear(renderer); pasa el rodillo por toda la pantalla: Toma ese color azul oscuro y pinta los 800x600 píxeles del Lienzo Trasero (Back Buffer) de un solo golpe.
¿Para qué sirve? Para tapar el dibujo viejo del fotograma anterior. Si no pasaras este rodillo azul, el cuadrado verde dejaría una estela manchada infinita en la pantalla a medida que avanza...
Luego, ejecuta la línea: <player->Render(renderer);> donde se ejecuta el método Render() de la Clase GameObject pasándole por parámetro el renderer. Desde la Clase GameObject lo que hace es ejecutar el método Render() de cada componente, pero el único que tiene éste método sobreescrito es SquareRenderer, y lo que hace es crear un rectángulo con las coordenadas en x, y que en ese instante tenga el Transform de player, el ancho y alto. Luego lo dibuja y lo pinta.
Y luego con ésta línea <SDL_RenderPresent(renderer);> desde la Clase Game.cpp, lo que hace es hacerlo visible para el usuario.

> [5] Entender exactamente cómo funciona el borrado y re pintado de pantalla:
1) En el método interno Render() de la Clase Game.cpp cuando llegamos a la 1era. línea: <SDL_SetRenderDrawColor(renderer, 30, 35, 45, 255);> lo que hace es eligir el color del pincel. Le dice a la GPU: «Moja el pincel en pintura azul oscuro (R:30, G:35, B:45)».
2) Cuando pasamos a la línea siguiente: <SDL_RenderClear(renderer);> ESTA es la línea que realmente PINTA todo el fondo! Toma el rodillo con el color que elegiste en la 1era. línea, que fue (azul oscuro) y lo pasa por el 100% de la pantalla de un solo golpe. Al pintar toda la pantalla de azul oscuro, tapa y borra el dibujo viejo del fotograma anterior. Se llama "Clear" (limpiar), es decir, limpia la tela pintándola entera de un solo color base.
3) En la línea N°3: <player->Render(renderer);> ejecuta el método Render() de la Clase GameObject ya que player es un GameObject y le pasa por argumento el "pincel". En éste método de la Clase GameObject, lo que hace en el método Render(), es ejecutar el método <component.Render(renderer);> pasándole por argumento el mismo pincel que viene desde la Clase Game.cpp a la Clase SquareRenderer que es el componente que tiene sobreescrito el método Render() para pintar en pantalla al personaje (el Rectángulo verde). Pinta el Rectángulo verde en la coordenada y con las dimensiones sobre lienzo oculto (aún no se ve en pantalla). ¿Cómo? Bueno, previamente creamos el Rectángulo en las coordenadas y con las dimensiones deseadas:
SDL_Rect rect = {
  static_cast<int>(transform->x),
  static_cast<int>(transform->y),
  transform->width,
  transform->height
};
Luego, en esta línea: <SDL_SetRenderDrawColor(renderer, r, g, b, a);> 👉 Cambia el pincel: ahora lo moja en pintura verde.
Luego en la siguiente: <SDL_RenderFillRect(renderer, &rect);> 👉 Pinta el cuadrado verde directamente sobre el lienzo oculto (que aún NO se muestra en pantalla). En la memoria de la tarjeta gráfica, esos píxeles específicos (del 50 al 150) dejan de ser azules y pasan a ser verdes en ese preciso nanosegundo.
4) Y finalmente, en la línea: <SDL_RenderPresent(renderer);> La tarjeta gráfica hace un intercambio instantáneo de lienzos, y muestra el lienzo oculto que hemos pintado nuevamente el rectángulo verde con otras coordenadas y mismas dimensiones.