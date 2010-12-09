/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus - util.c                                                  *
 *   Mupen64Plus homepage: http://code.google.com/p/mupen64plus/           *
 *   Copyright (C) 2002 Hacktarux                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 * Provides common utilities to the rest of the code:
 *  -String functions
 *  -Doubly-linked list
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>
#include <SDL.h>

#include "rom.h"
#include "util.h"
#include "translate.h"

/** trim
 *    Removes leading and trailing whitespace from str. Function modifies str
 *    and also returns modified string.
 */
char *trim(char *str)
{
    int i;
    char *p = str;

    while (isspace(*p))
        p++;

    if (str != p)
        {
        for (i = 0; i <= strlen(p); ++i)
            str[i]=p[i];
        }

    p = str + strlen(str) - 1;
    if (p > str)
    {
        while (isspace(*p))
            p--;
        p[1] = '\0';
    }

    return str;
}

char* strnstrip(char* string, int size)
{
    int counter, place;
    for (counter = place = 0; counter < size && string[counter] != '\0'; counter++)
        {
        string[place] = string[counter];
        if (string[counter] != ' ')
            place++;
        }
    string[place] = '\0';
    return string;
}

/** event_to_str
 *    Creates a string representation of an SDL input event. If the event is
 *    not supported by this function, NULL is returned.
 *
 *    Notes:
 *     -This function assumes SDL events are already initialized.
 *     -It is up to the caller to free the string memory allocated by this
 *      function.
 */
char *event_to_str(const SDL_Event *event)
{
    char *event_str = NULL;

    switch(event->type)
    {
        case SDL_JOYAXISMOTION:
            if(event->jaxis.value >= 15000 || event->jaxis.value <= -15000)
            {
                event_str = malloc(10);
                snprintf(event_str, 10, "J%dA%d%c",
                         event->jaxis.which,
                     event->jaxis.axis,
                     event->jaxis.value > 0? '+' : '-');
            }
            break;

        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
            event_str = malloc(10);
            snprintf(event_str, 10, "J%dB%d",
                     event->jbutton.which,
                     event->jbutton.button);
            break;

        case SDL_JOYHATMOTION:
            event_str = malloc(10);
            snprintf(event_str, 10, "J%dH%dV%d",
                     event->jhat.which,
                     event->jhat.hat,
                     event->jhat.value);
            break;
    }

    return event_str;
}

/** event_active
 *    Returns 1 if the specified joystick event is currently active. This
 *    function expects an input string of the same form output by event_to_str.
 */
int event_active(const char* event_str)
{
    char device, joy_input_type, axis_direction;
    int dev_number, input_number, input_value;
    SDL_Joystick *joystick = NULL;

    /* Empty string. */
    if(!event_str||strlen(event_str)==0)
        return 0;

    /* Parse string depending on type of joystick input. */
    if(event_str[0]=='J')
        {
        switch(event_str[2])
            {
            /* Axis. */
            case 'A':
                sscanf(event_str, "%c%d%c%d%c", &device, &dev_number,
                       &joy_input_type, &input_number, &axis_direction);
                break;
            /* Hat. ??? */
            case 'H':
                sscanf(event_str, "%c%d%c%dV%d", &device, &dev_number,
                       &joy_input_type, &input_number, &input_value);
                break;
            /* Button. */
            case 'B':
                sscanf(event_str, "%c%d%c%d", &device, &dev_number,
                       &joy_input_type, &input_number);
                break;
            }

        joystick = SDL_JoystickOpen(dev_number);
        SDL_JoystickUpdate();
        switch(joy_input_type)
            {
            case 'A':
                if(axis_direction=='-')
                    return SDL_JoystickGetAxis(joystick, input_number) < -15000;
                else
                    return SDL_JoystickGetAxis(joystick, input_number) > 15000;
                return (int)SDL_JoystickGetButton(joystick, input_number);
                break;
            case 'B':
                return (int)SDL_JoystickGetButton(joystick, input_number);
                break;
            case 'H':
                return SDL_JoystickGetHat(joystick, input_number) == input_value;
                break;
            }
        }

    /* TODO: Keyboard event. */
    /* if(event_str[0]=='K') */

    /* Undefined event. */
    return 0;
}

/** key_pressed
 *   Returns 1 if the given key is currently pressed.
 */
int key_pressed(SDLKey k)
{
    Uint8 *key_states;
    int num_keys;

    SDL_PumpEvents(); // update input state array
    key_states = SDL_GetKeyState(&num_keys);

    if(k >= num_keys)
        return 0;

    return key_states[k];
}

/** file utilities **/

/** isfile
 *    Returns TRUE if given file path exists and is a regular file
 */
int isfile(char *path)
{
    struct stat sbuf;

    return (stat(path, &sbuf) == 0) && S_ISREG(sbuf.st_mode);
}

/** isdir
 *    Returns TRUE if given file path exists and is a directory
 */
int isdir(char *path)
{
    struct stat sbuf;

    return (stat(path, &sbuf) == 0) && S_ISDIR(sbuf.st_mode);
}

/** copyfile
 *    copies file at src to a new file dest. If dest exists, its contents will be truncated and replaced.
 */
int copyfile(char *src, char *dest)
{
    FILE *to, *from;
    char c;

    if((from = fopen(src, "r")) == NULL)
        return -1;

    if((to = fopen(dest, "w")) == NULL)
    {
        fclose(from);
        return -2;
    }

    while(!feof(from))
    {
        c = fgetc(from);
        if(ferror(from))
        {
            fclose(from);
            fclose(to);
            unlink(dest);
            return -3;
        }
        if(!feof(from))
            fputc(c, to);

        if(ferror(to))
        {
            fclose(from);
            fclose(to);
            unlink(dest);
            return -4;
        }
    }

    fclose(from);
    fclose(to);

    return 0;
}

/** linked list functions **/

/** list_prepend
 *    Allocates a new list node, attaches it to the beginning of list and sets the
 *    node data pointer to data.
 *    Returns - the new list node.
 */
list_node_t *list_prepend(list_t *list, void *data)
{
    list_node_t *new_node,
                *first_node;

    if(list_empty(*list))
    {
        (*list) = malloc(sizeof(list_node_t));
        (*list)->data = data;
        (*list)->prev = NULL;
        (*list)->next = NULL;
        return *list;
    }

    // create new node and prepend it to the list
    first_node = *list;
    new_node = malloc(sizeof(list_node_t));
    first_node->prev = new_node;
    *list = new_node;

    // set members in new node and return it
    new_node->data = data;
    new_node->prev = NULL;
    new_node->next = first_node;

    return new_node;
}

/** list_append
 *    Allocates a new list node, attaches it to the end of list and sets the
 *    node data pointer to data.
 *    Returns - the new list node.
 */
list_node_t *list_append(list_t *list, void *data)
{
    list_node_t *new_node,
                *last_node;

    if(list_empty(*list))
    {
        (*list) = malloc(sizeof(list_node_t));
        (*list)->data = data;
        (*list)->prev = NULL;
        (*list)->next = NULL;
        return *list;
    }

    // find end of list
    last_node = *list;
    while(last_node->next != NULL)
        last_node = last_node->next;

    // create new node and return it
    last_node->next = new_node = malloc(sizeof(list_node_t));
    new_node->data = data;
    new_node->prev = last_node;
    new_node->next = NULL;

    return new_node;
}

/** list_node_delete
 *    Deallocates and removes given node from the given list. It is up to the
 *    user to free any memory allocated for the node data before calling this
 *    function. Also, it is assumed that node is an element of list.
 */
void list_node_delete(list_t *list, list_node_t *node)
{
    if(node == NULL || *list == NULL) return;

    if(node->prev != NULL)
        node->prev->next = node->next;
    else
        *list = node->next; // node is first node, update list pointer

    if(node->next != NULL)
        node->next->prev = node->prev;

    free(node);
}

/** list_delete
 *    Deallocates and removes all nodes from the given list. It is up to the
 *    user to free any memory allocated for all node data before calling this
 *    function.
 */
void list_delete(list_t *list)
{
    list_node_t *prev = NULL,
                *curr = NULL;

    // delete all list nodes in the list
    list_foreach(*list, curr)
    {
        if(prev != NULL)
            free(prev);

        prev = curr;
    }
    
    // the last node wasn't deleted, do it now
    if (prev != NULL)
        free(prev);

    *list = NULL;
}

/** list_node_move_front
 *    Moves the given node to the first position of list. It is assumed that
 *    node is an element of list.
 */
void list_node_move_front(list_t *list, list_node_t *node)
{
    list_node_t *tmp;

    if(node == NULL ||
       *list == NULL ||
       node == *list)
        return;

    tmp = *list;
    node->prev->next = node->next;
    if(node->next != NULL)
        node->next->prev = node->prev;
    node->prev = NULL;
    node->next = *list;
    (*list)->prev = node;
    *list = node;
}

/** list_node_move_back
 *    Moves the given node to the last position of list. It is assumed that
 *    node is an element of list.
 */
void list_node_move_back(list_t *list, list_node_t *node)
{
    list_node_t *tmp;

    tmp = list_last_node(*list);

    if(node == NULL ||
       *list == NULL ||
       node == tmp)
        return;

    node->next->prev = node->prev;
    if(node->prev != NULL)
        node->prev->next = node->next;
    else
        *list = node->next; // first node is being moved, update list pointer
    tmp->next = node;
    node->prev = tmp;
    node->next = NULL;
}

/** list_nth_node_data
 *    Returns the nth node in list. If n is out of range, NULL is returned.
 */
void *list_nth_node_data(list_t list, int n)
{
    list_node_t *curr = NULL;

    list_foreach(list, curr)
    {
        if(n-- == 0)
            break;
    }

    return curr != NULL ? curr->data : curr;
}

/** list_first_node
 *    Returns the first node in list.
 */
list_node_t *list_first_node(list_t list)
{
    return list;
}

/** list_first_data
 *    Returns the data pointer of the first node in list.
 */
void *list_first_data(list_t list)
{
    if(list) return list->data;
    return NULL;
}

/** list_last_node
 *    Returns the last node in list.
 */
list_node_t *list_last_node(list_t list)
{
    if(list != NULL)
    {
        while(list->next != NULL)
            list = list->next;
    }

    return list;
}

/** list_last_data
 *    Returns the data pointer of the last node in list.
 */
void *list_last_data(list_t list)
{
    list_node_t *node = list_last_node(list);
    if(node) return node->data;
    return NULL;
}

/** list_empty
 *    Returns 1 if list is empty, else 0.
 */
int inline list_empty(list_t list)
{
    return list == NULL;
}

/** list_length
 *    Returns the number of elements in list
 */
int list_length(list_t list)
{
    int len = 0;
    list_node_t *curr;

    list_foreach(list, curr)
    {
        len++;
    }

    return len;
}

/** list_find_node
 *    Searches the given list for a node whose data pointer matches the given data pointer.
 *    If found, returns a pointer to the node, else, returns NULL.
 */
list_node_t *list_find_node(list_t list, void *data)
{
    list_node_t *node = NULL;

    list_foreach(list, node)
        if(node->data == data)
            break;

    return node;
}

void countrycodestring(unsigned short countrycode, char *string)
{
    switch (countrycode)
    {
    case 0:    /* Demo */
        strcpy(string, tr("Demo"));
        break;

    case '7':  /* Beta */
        strcpy(string, tr("Beta"));
        break;

    case 0x41: /* Japan / USA */
        strcpy(string, tr("USA/Japan"));
        break;

    case 0x44: /* Germany */
        strcpy(string, tr("Germany"));
        break;

    case 0x45: /* USA */
        strcpy(string, tr("USA"));
        break;

    case 0x46: /* France */
        strcpy(string, tr("France"));
        break;

    case 'I':  /* Italy */
        strcpy(string, tr("Italy"));
        break;

    case 0x4A: /* Japan */
        strcpy(string, tr("Japan"));
        break;

    case 'S':  /* Spain */
        strcpy(string, tr("Spain"));
        break;

    case 0x55: case 0x59:  /* Australia */
        sprintf(string, tr("Australia (0x%2.2X)"), countrycode);
        break;

    case 0x50: case 0x58: case 0x20:
    case 0x21: case 0x38: case 0x70:
        sprintf(string, tr("Europe (0x%02X)"), countrycode);
        break;

    default:
        sprintf(string, tr("Unknown (0x%02X)"), countrycode);
        break;
    }
}

void compressionstring(unsigned char compressiontype, char *string)
{
    switch (compressiontype)
    {
    case UNCOMPRESSED:
        strcpy(string, tr("Uncompressed"));
        break;
    case ZIP_COMPRESSION:
        strcpy(string, tr("Zip"));
        break;
    case GZIP_COMPRESSION:
        strcpy(string, tr("Gzip"));
        break;
    case BZIP2_COMPRESSION:
        strcpy(string, tr("Bzip2"));
        break;
    case LZMA_COMPRESSION:
        strcpy(string, tr("LZMA"));
        break;
    case SZIP_COMPRESSION:
        strcpy(string, tr("7zip"));
        break;
    default:
        string[0] = '\0';
    }
}

void imagestring(unsigned char imagetype, char *string)
{
    switch (imagetype)
    {
    case Z64IMAGE:
        strcpy(string, tr(".z64 (native)"));
        break;
    case V64IMAGE:
        strcpy(string, tr(".v64 (byteswapped)"));
        break;
    case N64IMAGE:
        strcpy(string, tr(".n64 (wordswapped)"));
        break;
    default:
        string[0] = '\0';
    }
}

void cicstring(unsigned char cic, char *string)
{
    switch (cic)
    {
    case CIC_NUS_6101:
        strcpy(string, tr("CIC-NUS-6101"));
        break;
    case CIC_NUS_6102:
        strcpy(string, tr("CIC-NUS-6102"));
        break;
    case CIC_NUS_6103:
        strcpy(string, tr("CIC-NUS-6103"));
        break;
    case CIC_NUS_6105:
        strcpy(string, tr("CIC-NUS-6105"));
        break;
    case CIC_NUS_6106:
        strcpy(string, tr("CIC-NUS-6106"));
        break;
    default:
        string[0] = '\0';
    }
}

void rumblestring(unsigned char rumble, char *string)
{
    switch (rumble)
    {
    case 1:
        strcpy(string, tr("Yes"));
        break;
    case 0:
        strcpy(string, tr("No"));
        break;
    default:
        string[0] = '\0';
    }
}

void savestring(unsigned char savetype, char *string)
{
    switch (savetype)
    {
    case EEPROM_4KB:
        strcpy(string, tr("Eeprom 4KB"));
        break;
    case EEPROM_16KB:
        strcpy(string, tr("Eeprom 16KB"));
        break;
    case SRAM:
        strcpy(string, tr("SRAM"));
        break;
    case FLASH_RAM:
        strcpy(string, tr("Flash RAM"));
        break;
    case CONTROLLER_PACK:
        strcpy(string, tr("Controller Pack"));
        break;
    case NONE:
        strcpy(string, tr("None"));
        break;
    default:
        string[0] = '\0';
    }
}

void playersstring(unsigned char players, char *string)
{
    if (players > 7)
        {
        string[0] = '\0';
        return;
        }

    unsigned short netplay=0;
    if (players > 4)
        {
        players-=3;
        netplay=1;
        }

    sprintf(string, "%d %s", players, (netplay) ? "Netplay" : "");
}

char* dirfrompath(const char* string)
{
    int stringlength, counter;
    char* buffer;

    stringlength = strlen(string);

    for(counter = stringlength; counter > 0; --counter)
        {
        if (string[counter-1] == '/')
            break;
        }

    buffer = (char*)malloc((counter+1)*sizeof(char));
    snprintf(buffer, counter+1, "%s", string);
    buffer[counter] = '\0';

    return buffer;
}



