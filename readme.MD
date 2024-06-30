# Zoombies

Zoombies es un juego 2D cuya mecanica principal es disparar mas rapido mientras mas zoom tengas.
Deberas balancear que tanto ves en tu pantalla con que tan rapido disparas mientras matas zombies y destruyes generadores.

## Cambios al motor

1. **Animacion de sprites**:  Se agrego la clase SpriteBatch y se modifico la clase Agent para efectuarlo
2. **Creaion de las balas**: Segun posicion del mouse y zoom de pantalla clases MainGame y Bullet 
3. **Gestion de vidas y multiples niveles**: Se uso el motor para agregar vidas y mas nivles al juego 
4. **QuadTree**: Se agrego un quadtree al motor, y se agrego en maingame se inicializa con los zombies y se usa al gestionar la infeccion y muerte de zombies por colision.
5. **Comportamiento de humanos**: los humanos tambien usan el quadtree inicializado con zombies para huir en direccion contraria al zombie mas cercano
6. **Otras funcionalidades menores** : spawners, hacks de niveles, modo dios deshabilitable, furia zombie (para que alcancen a los humanos).
