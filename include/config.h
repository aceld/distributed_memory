#ifndef __DSTORAGE_CONFIG_H_
#define __DSTORAGE_CONFIG_H_

/*   删除左边的空格   */
char * l_trim(char * szOutput, const char *szInput);


/*   删除右边的空格   */
char *r_trim(char *szOutput, const char *szInput);

/*   删除两边的空格   */
char * a_trim(char * szOutput, const char * szInput);

/* -------------------------------------------*/
/**
 * @brief  从配置文件中得到相对应的参数
 *
 * @param profile   配置文件路径
 * @param AppName   配置文件title名称[title]
 * @param KeyName   key
 * @param KeyVal    (OUT)  得到的value
 *
 * @returns   
 *      0 succ, -1 fail
 */
/* -------------------------------------------*/
int GetProfileString(char *profile, char *AppName, char *KeyName, char *KeyVal );

#endif
