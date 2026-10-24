# Hola iPhone (SDL3)

Programa minimo para validar la cadena de herramientas: compilar en la nube
(GitHub Actions, sin Mac) e instalar en iPhone desde Windows.

## Pasos
1. Crea un repositorio nuevo en GitHub y sube TODO el contenido de esta carpeta
   (incluida la carpeta oculta `.github`).
2. En el repo: pestana **Actions** -> workflow "Build iOS" -> espera a que termine
   (tarda unos minutos la primera vez).
3. Entra a la ejecucion terminada y descarga el artefacto **holaiphone-ipa**
   (es un zip que contiene `holaiphone.ipa`).
4. En Windows abre **Sideloadly**, conecta el iPhone por USB, arrastra el `.ipa`,
   pon tu Apple ID y pulsa Start.
5. En el iPhone: Ajustes -> General -> VPN y gestion de dispositivos -> confiar
   en tu Apple ID. Si te lo pide, activa el Modo Desarrollador (Ajustes ->
   Privacidad y seguridad).
6. Abre la app: veras un fondo que cambia de color y un cuadro blanco que sigue tu dedo.

Con Apple ID gratuito la app caduca a los 7 dias.
