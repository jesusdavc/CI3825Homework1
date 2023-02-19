
//
//  main.c
//  sistemas
//
//  Created by Jhonaiker Blanco on 16/2/23.
//  Copyright © 2023 Jhonaiker Blanco. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


//Esta funcion obtiene el hash del usuario
int getHash(char text[20]){
    int counter = 0;
    
    for(int i = 0; i < strlen(text); i++){
        counter = counter + text[i];
    }
    return counter % 20;
}

// Esta funcion limpia el buffer
void cleanBuffer(){
    int c;
    do{
        c = getchar();
    }
    while(c != EOF && c != '\n');
}

// Lista enlazada que sirve para guardar los usuarios seguidos por alguien
struct userFollowed {
    char username[20];
    struct userFollowed* nextUserFollowed;
};

// Lista enlazada que guarda tweets
struct tweet {
    char text[280];
    time_t timeOfTweet;
    struct tweet* nextTweet;
    char username[20];
};

// Estructura enlazada que guarda datos de un usuario
struct userElement {
    char username[20];
    char description[100];
    int password;
    struct tweet nextTweet;
    struct userFollowed nextUserFollowed;
};

// Funcion que revisa si un usiario sigue a otro, regresa 1 si lo sigue, regresa 0 si no lo sigue
int checkIfUserIsFollowed (struct userFollowed* userFollowedToCheck, char userToCheckIfItsFollowed[20]) {
    int result = 0;
    
    while (userFollowedToCheck != NULL) {
        if(strcmp(userFollowedToCheck->username,userToCheckIfItsFollowed) == 0){
            result = 1;
            break;
        }
        userFollowedToCheck = userFollowedToCheck->nextUserFollowed;
    }
    
    return result;
}

// Funcion para seguir a un usuario
struct userFollowed * addUserToFollowersList(struct userFollowed *firstUserFollowed, char newUser[20]) {
    struct userFollowed *currentUserFollowed = firstUserFollowed;
    struct userFollowed *newUserToFollow;
    while ( currentUserFollowed != NULL && currentUserFollowed->nextUserFollowed != NULL) {
        currentUserFollowed = currentUserFollowed->nextUserFollowed;
    }
    
    newUserToFollow = (struct userFollowed *) malloc(sizeof(struct userFollowed));
    strcpy(newUserToFollow->username, newUser);
    newUserToFollow->nextUserFollowed= NULL;
    if (currentUserFollowed != NULL)
        currentUserFollowed->nextUserFollowed = newUserToFollow;
    else
        firstUserFollowed = newUserToFollow;
    return firstUserFollowed;
}

// Funcion para añadir un tweet a la lista de tweets de un usario
void addTweetToList(struct tweet * firstTweetOfUser, char newTweet[280], char userThatSentTheTweet[20]) {
    
    struct tweet *currentTweetInList = firstTweetOfUser;
    struct tweet *newTweetToAdd;
    
    if(firstTweetOfUser == NULL){
        
        newTweetToAdd->nextTweet = NULL;
        newTweetToAdd->timeOfTweet = time(0);
        strcpy(newTweetToAdd->text, newTweet);
        strcpy(newTweetToAdd->username, userThatSentTheTweet);
        
        firstTweetOfUser = newTweetToAdd;
    } else {
        
        while ( currentTweetInList != NULL && currentTweetInList->nextTweet != NULL) {
            currentTweetInList = currentTweetInList->nextTweet;
        }
        
        newTweetToAdd = (struct tweet *) malloc(sizeof(struct tweet));
        strcpy(newTweetToAdd->text, newTweet);
        strcpy(newTweetToAdd->username, userThatSentTheTweet);
        newTweetToAdd->nextTweet = NULL;
        newTweetToAdd->timeOfTweet = time(0);
        if (currentTweetInList != NULL)
            currentTweetInList->nextTweet = newTweetToAdd;
        else
            firstTweetOfUser = newTweetToAdd;
    }
}



// Funcion para mostrar los tweets de un usuario
void showTweetsOfUser (struct tweet * currentTweet) {
    
    int numberOfCurrentTweet = 0;
    if(currentTweet->nextTweet != NULL){
        currentTweet = currentTweet->nextTweet;
    }
    
    while ( currentTweet != NULL) {
        numberOfCurrentTweet++;
        printf("Tweet number %d: %s \n", numberOfCurrentTweet, currentTweet->text);
        currentTweet = currentTweet->nextTweet;
    }
    
    if(numberOfCurrentTweet == 0){
        printf("This user does not have tweets \n");
    }
}

// Funcion para añadir un tweet a la timeline
struct tweet * addTweetToTimeline (struct tweet * headTweet,struct tweet * tweetToAddToTimeline) {
    struct tweet *newTweetToAdd;
    struct tweet *currentTweetChecked = headTweet;
    newTweetToAdd = (struct tweet *) malloc(sizeof(struct tweet));
    strcpy(newTweetToAdd->username, tweetToAddToTimeline->username);
    strcpy(newTweetToAdd->text, tweetToAddToTimeline->text);
    newTweetToAdd->timeOfTweet = tweetToAddToTimeline->timeOfTweet;
    
    if(headTweet == NULL){
        headTweet = newTweetToAdd;
    } else if( difftime(headTweet->timeOfTweet, newTweetToAdd->timeOfTweet) <= 0 ){
        newTweetToAdd->nextTweet = headTweet;
        headTweet = newTweetToAdd;
    } else {
        while(1 == 1 && currentTweetChecked != NULL){
            if(difftime(currentTweetChecked->timeOfTweet, newTweetToAdd->timeOfTweet) < 0 && currentTweetChecked->nextTweet == NULL){
                currentTweetChecked->nextTweet = newTweetToAdd;
                break;
            } else if(difftime(newTweetToAdd->timeOfTweet , currentTweetChecked->timeOfTweet) >= 0 && difftime(currentTweetChecked->nextTweet->timeOfTweet , newTweetToAdd->timeOfTweet) >= 0){
                newTweetToAdd-> nextTweet = currentTweetChecked->nextTweet;
                currentTweetChecked->nextTweet = newTweetToAdd;
                
                break;
            } else {
                currentTweetChecked = currentTweetChecked->nextTweet;
            }
        }
    }
    
    return headTweet;
};

// Funcion para añadir tweets de un usuario a la timeline
struct tweet * addTweetsOfUserToTimeline (struct tweet * headTweet,struct tweet * firstTweetOfUser){
    
    if(firstTweetOfUser->nextTweet != NULL){
        firstTweetOfUser = firstTweetOfUser->nextTweet;
    }
    
    while(firstTweetOfUser != NULL){
        headTweet = addTweetToTimeline(headTweet, firstTweetOfUser);
        firstTweetOfUser = firstTweetOfUser->nextTweet;
    }
    
    return headTweet;
}

// Funcion para mostrar los seguidores de un usuario
void showFollowersOfUser (struct userFollowed * currentUser) {
    
    int numberOfCurrentFollowers = 0;
    if(currentUser->nextUserFollowed != NULL){
        currentUser = currentUser->nextUserFollowed;
    }
    
    while ( currentUser != NULL) {
        numberOfCurrentFollowers++;
        printf("Seguidor %d: %s \n", numberOfCurrentFollowers, currentUser->username);
        currentUser = currentUser->nextUserFollowed;
    }    
}


int main(int argc, const char * argv[]) {
    
    // Hash list que guarda usuarios
    struct userElement usersInProgam[20][5];
    
    //Cantidad de usuario por fila de lista enlazada
    int amountOfUsersByHash[20];
    
    // description de un usuario que se esta registrando
    char description[100];
    
    // Username de un usuario que se esta registrando
    char newUsername[20];
    
    // password de un usuario que se esta registrando
    char newPassword[20];
    
    
    // Username de un usuario que se esta logeando
    char username[20];
    
    // password de un usuario que se esta logeando
    char password[20];
    
    // String que revisa si un usuario quiere seguir a otro o no (su valor será yes o no)
    char wantToFollow[10];
    
    // Action que realiza en usuario en twitter (login signup o leave)
    char action[10];
    
    // Valor del hash del username del usuario
    int currentHashValue = 0;
    
    //Cantidad de usuarios en fila actual de la lista enlazada
    int amountOfUsersInCurrentHash = 0;
    
    //Positcion de un usuario en la columna de la lista enlazada
    int positionInHashOfUser = 0;
    
    // Checkea si un username ya está siendo usado, si su valor es 1 está siendo usado, si vale 0 no
    int userTaken = 0;
    
    // Checkea si el usuario A ya sigue al usuario B, si vale 1 si, si vale 0 no
    int isUserFollowed = 0;
    
    
    
    
    
    // Este bucle le pregunta al usuario que accion quiere realizar
    while(strcmp("leave", action) != 0){
        printf("DON'T MISS WHAT’S HAPPENING! LOGIN, SIGNUP OR LEAVE\n");
        strcpy(action, "");
        scanf("%s", action);
        
        if(strcmp("signup", action) == 0){
            
            printf("Username: ");
            scanf("%s", newUsername);
            cleanBuffer();
            
            printf("Password: ");
            scanf("%s", newPassword);
            cleanBuffer();
            
            printf("Description: ");
            fgets(description,100, stdin);
            
            currentHashValue = getHash(newUsername);
            
            amountOfUsersInCurrentHash = amountOfUsersByHash[currentHashValue];
            
            userTaken = 0;
            
            // Este bucle revisa si el username está ocupado
            for(int i = 0; i < amountOfUsersInCurrentHash; i++){
                if(strcmp(usersInProgam[amountOfUsersInCurrentHash][i].username, newUsername) == 0){
                    userTaken = 1;
                    break;
                }
            }
            
            // Nombre de usuario ocupado
            if(userTaken == 1){
                printf("Username already taken \n");
            }
            // Se está creando un usuario
            else {
                strcpy(usersInProgam[currentHashValue][amountOfUsersInCurrentHash].username, newUsername);
                strcpy(usersInProgam[currentHashValue][amountOfUsersInCurrentHash].description, description);
                usersInProgam[currentHashValue][amountOfUsersInCurrentHash].password = getHash(newPassword);
                amountOfUsersByHash[currentHashValue] = amountOfUsersByHash[currentHashValue] + 1;
            }
            // Login
        } else if(strcmp("login", action) == 0){
            
            printf("Username: ");
            scanf("%s", username);
            cleanBuffer();
            
            currentHashValue = getHash(username);
            amountOfUsersInCurrentHash = amountOfUsersByHash[currentHashValue];
            
            
            userTaken = 0;
            
            // Revisar si hay un usuario con el username en la base de datos y guardar su posicion
            for(int i = 0; i < amountOfUsersInCurrentHash; i++){
                if(strcmp(usersInProgam[currentHashValue][i].username, username) == 0){
                    userTaken = 1;
                    positionInHashOfUser = i;
                    break;
                }
            }
            
            // Se checkea si el usuario existe o no, en caso de que no se volvera a la pantalla de inicio, en caso de que si se le preguntara
            // la contraseña
            if(userTaken == 1){
                
                printf("Password: ");
                scanf("%s", password);
                cleanBuffer();
                
                if(usersInProgam[currentHashValue][positionInHashOfUser].password == getHash(password)){
                    printf("Login successfully \n");
                    
                    // Show timeline
                    
                    struct userElement *currentUser = &usersInProgam[currentHashValue][positionInHashOfUser];
                    
                    struct userFollowed userFollowedList = currentUser->nextUserFollowed;
                    
                    struct userFollowed * nextUserFollowed = userFollowedList.nextUserFollowed;
                    
                    int hashOfUserOfTweets = 0;
                    int positionInArrayOfUserOfTweets = 0;
                    struct userElement userOfTweets;
                    struct tweet * headTweet = (struct tweet *) malloc(sizeof(struct tweet));
                    
                    // Este while obtiene los cuentas que el usuario sigue y crea la timeline en base a ellas buscando su posicion en la hash list
                    // que estamos usando como base de datos
                    while(nextUserFollowed != NULL){
                        
                        hashOfUserOfTweets = getHash(nextUserFollowed->username);
                        
                        for(int i = 0; i < amountOfUsersByHash[hashOfUserOfTweets]; i++){
                            if(strcmp(nextUserFollowed->username, usersInProgam[hashOfUserOfTweets][i].username)){
                                positionInArrayOfUserOfTweets = i;
                                break;
                            }
                        }
                        
                        userOfTweets = usersInProgam[hashOfUserOfTweets][positionInArrayOfUserOfTweets];
                        
                        headTweet = addTweetsOfUserToTimeline(headTweet, &usersInProgam[hashOfUserOfTweets][positionInArrayOfUserOfTweets].nextTweet);
                        
                        nextUserFollowed = nextUserFollowed->nextUserFollowed;
                    }
                    
                    // Este while muestra uno por uno los tweets de la timeline guardados en el while anterior
                    while(headTweet != NULL){
                        //numberOfTweetsShowed++;
                        //printf("Tweet number %d \n", numberOfTweetsShowed);
                        if(strcmp(headTweet->username,"") != 0  && strcmp(headTweet->text, "") != 0){
                            printf("-%s: %s \n",headTweet->username,headTweet->text);
                        }
                        headTweet = headTweet->nextTweet;
                    }
                    
                    char nextAction[10] = "";
                    
                    // Este while le pregunta al usuario que quiere hacer hasta que haga logout
                    while(strcmp("logout", nextAction) != 0){
                        printf("WHAT’S HAPPENING? \n");
                        scanf("%s", nextAction);
                        cleanBuffer();
                        
                        // Este if sirve para crear un tweet
                        if(strcmp("+", nextAction) == 0){
                            
                            char textOfNewTweet[280];
                            
                            printf("You can write a new tweet \n");
                            printf("New tweet: ");
                            fgets(textOfNewTweet,280, stdin);
                            
                            addTweetToList(&usersInProgam[currentHashValue][positionInHashOfUser].nextTweet, textOfNewTweet, usersInProgam[currentHashValue][positionInHashOfUser].username);
                            
                            // En este caso se entra al perfil de un usuario
                        } else if(strcmp("@", nextAction) == 0){
                            
                            
                            char userToCheck[20];
                            printf("Check a user: ");
                            scanf("%s", userToCheck);
                            cleanBuffer();

                            
                            int currentHashValueToCheck = getHash(userToCheck);
                            int amountOfUsersInCurrentHashToCheck = amountOfUsersByHash[currentHashValueToCheck];
                            
                            int userExists = 0;
                            int positionInHashOfUserToCheck = 0;
                            isUserFollowed = 0;
                            
                            // Aqui se busca el lugar de la base de datos del usuario cuyo perfil está siendo buscando y también se revisa si existe o no
                            for(int i = 0; i < amountOfUsersInCurrentHashToCheck; i++){
                                if(strcmp(usersInProgam[currentHashValueToCheck][i].username, userToCheck) == 0){
                                    userExists = 1;
                                    positionInHashOfUserToCheck = i;
                                    break;
                                }
                            }
                            
                            // Caso en el que si existe un usuario con el nombre a buscar en la base de datos (la hash table)
                            if(userExists){
                                
                                //Mostrar tweets de ese usuario
                                showTweetsOfUser(&usersInProgam[currentHashValueToCheck][positionInHashOfUserToCheck].nextTweet);
                                // Mostrar Seguidores de ese usuario
                                showFollowersOfUser(&usersInProgam[currentHashValueToCheck][positionInHashOfUserToCheck].nextUserFollowed);
                                
                                // El usuario no puede seguirse a si mismo
                                if(strcmp(userToCheck,username) == 0){
                                    printf("Your description is: %s \n", usersInProgam[currentHashValueToCheck][positionInHashOfUserToCheck].description);
                                    printf("This is your own profile \n");
                                } else {
                                    printf("The user exists! \n");
                                    printf("Its description is: %s \n", usersInProgam[currentHashValueToCheck][positionInHashOfUserToCheck].description);
                                    
                                    currentHashValue = getHash(username);
                                    for(int i = 0; i < amountOfUsersByHash[currentHashValue]; i++){
                                        if(strcmp(usersInProgam[currentHashValue][i].username, username) == 0){
                                            userExists = 1;
                                            positionInHashOfUser = i;
                                            break;
                                        }
                                    }
                                    
                                    // Revisar si el usuario ya sigue este perfil
                                    isUserFollowed = checkIfUserIsFollowed(&usersInProgam[currentHashValue][positionInHashOfUser].nextUserFollowed, userToCheck);
                                    
                                    // Si el usuario ya sigue este perfil se le dirá eso
                                    if(isUserFollowed == 1){
                                        printf("You already follow this user \n");
                                    }
                                    // Si no sigue a este perfil se le preguntará si quiere hacerlo
                                    else {
                                        
                                        strcpy(wantToFollow,"");
                                        while(strcmp("no", wantToFollow) != 0 && strcmp("yes", wantToFollow) != 0){
                                            printf("Do you want to follow to this user? write 'yes' or 'no' \n");
                                            scanf("%s", wantToFollow);
                                            cleanBuffer();

                                            
                                            // Si el usuario quiere seguir este perfil este caso hara que lo haga
                                            if(strcmp("yes", wantToFollow) == 0){
                                                printf("Now you follow this user \n");
                                                addUserToFollowersList(&usersInProgam[currentHashValue][positionInHashOfUser].nextUserFollowed, userToCheck);
                                            }
                                            // Si el usuario no quiere seguir este perfil se mostrara esto
                                            else if(strcmp("no", wantToFollow) == 0){
                                                printf("You don't want to follow this user \n");
                                            }
                                            //Si el usuario pone un caso no valido se le preguntará otra vez
                                            else {
                                                printf("This command does not exist \n");
                                            }
                                        }
                                    }
                                    
                                }
                            }
                            // Si usuario intenta seguir a un usuario que no existe se le mostrara esto
                            else {
                                printf("This user does not exist \n");
                            }
                            
                            //Si el usuario hace logout saldrá del perfil
                        } else if(strcmp("logout", nextAction) == 0){
                            printf("You logout from your account \n");
                        }
                        // Si el usuario pone un comando no valido se le preguntará otra vez que hacer
                        else {
                            printf("That command does not exist \n");
                        }
                    }
                    // Si el usuario pone mal la clave al hacer login se le mostrara esto
                } else {
                    printf("Wrong password \n");
                }
                // Si el usuario intenta hacer login de un perfil que no existe se le mostrará eso
            } else {
                printf("There is not a user with that username \n");
            }
            // Si el usuario elige cerrar el programa se le mostrará esto
        } else if(strcmp("leave", action) == 0){
            printf("You leaved the app\n");
        }
        // Si el usuario hace una accion no valida se le mostrará esto
        else {
            printf("Unvalid action \n");
        }
    }
    
    return 0;
}
