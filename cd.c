/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zserobia <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 14:23:13 by zserobia          #+#    #+#             */
/*   Updated: 2024/11/23 14:23:15 by zserobia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "parser.h"


// Прототипы функций
char    *get_env_value(t_env *env, const char *var_name);
void    update_env(t_env *env, const char *var_name, const char *new_value);
void    cd_to_path(const char *path, t_env *env);
void    cd_to_home(t_env *env);
void    cd_to_previous(t_env *env);


// Обновить переменную окружения или добавить новую
void update_env(t_env *env, const char *var_name, const char *new_value) {
    while (env) {
        if (strcmp(env->var, var_name) == 0) {
            free(env->value);
            env->value = strdup(new_value);
            return;
        }
        env = env->next;
    }
    // Если переменная не найдена, добавить её (реализуйте при необходимости)
    fprintf(stderr, "bash: Environment variable %s not found. Please implement insertion.\n", var_name);
}

// Перейти в указанную директорию
void cd_to_path(const char *path, t_env *env)
{
    char *current_path = getcwd(NULL, 0); // Текущая директория

    if (!current_path) {
        perror("bash: cd: error getting current directory");
        return;
    }

    if (chdir(path) == -1) { // Перемещение в директорию
        printf(stderr, "cd: %s: ", path);
    } else {
        char *new_path = getcwd(NULL, 0); // Получаем новую директорию
        if (new_path) {
            update_env(env, "OLDPWD", current_path); // Обновляем OLDPWD
            update_env(env, "PWD", new_path);       // Обновляем PWD
            free(new_path);
        }
    }
    free(current_path);
}

// Получить значение переменной окружения
char *get_env_value(t_env *env, const char *var_name)
{
    while (env)
    {
        if (strcmp(env->var, var_name) == 0)
            return env->value;
        env = env->next;
    }
    return NULL;
}

// Перейти в домашнюю директорию
void cd_to_home(t_env *env)
{
    char *home_path;
    home_path = get_env_value(env, "HOME");

    if (!home_path)
    {
        printf("bash: cd: HOME not set\n");
        return;
    }

    cd_to_path(home_path, env);
}

// Перейти в предыдущую директорию
void cd_to_previous(t_env *env)
{
    char *old_path;

    old_path = get_env_value(env, "OLDPWD");

    if (!old_path)
    {
        printf("bash: cd: OLDPWD not set\n");
        return;
    }

    cd_to_path(old_path, env);
    printf("%s\n", old_path); // Печатаем предыдущую директорию
}

void builtin_cd(char **args, t_env *env)
{
    int arg_count;

    arg_count = 0;

    // Подсчет аргументов
    while (args[arg_count])
        arg_count++;

    // Слишком много аргументов
    if (arg_count > 2)
    {
        printf("bash: cd: too many arguments\n");
        g_global_exit = 1;
    }
    // Если аргументов нет или только "--", переходим в домашнюю директорию
    else if (arg_count == 1 || (arg_count == 2 && strcmp(args[1], "--") == 0))
        cd_to_home(env);
    // Если аргумент "-", переходим в предыдущую директорию
    else if (arg_count == 2 && strcmp(args[1], "-") == 0)
        cd_to_previous(env);
    // В остальных случаях переходим в указанную директорию
    else
        cd_to_path(args[1], env);
}
