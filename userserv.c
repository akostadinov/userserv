#include <string.h>

#include <debug.h>
#include <plugin.h>
#include <prpl.h>
#include <version.h>

#define PLUGIN_ID   "userserv"
#define VERSION     "0.1"

static PurplePluginProtocolInfo *irc_info = NULL;

static void
signed_on(PurpleConnection *connection)
{
    PurpleAccount *account;
    const char *username;
    const char *server;
    const char *password;
    gchar *msg;

    account = purple_connection_get_account(connection);

    if(strcmp("prpl-irc", purple_account_get_protocol_id(account)))
        return;

    username = purple_account_get_username(account);

    server = g_strrstr(username, "@");

    if(strcmp("@irc.devel.redhat.com", server))
        return;

    password = purple_account_get_password(account);

    if(password == NULL)
        return;

    // TODO: get the short username
    msg = g_strdup_printf("PRIVMSG USERSERV :login carlo %s\r\n", password);
    purple_debug_info("userserv", "sending %s\n", msg);
    irc_info->send_raw(connection, msg, strlen(msg));
    g_free(msg);
/*
    msg = g_strdup_printf("PRIVMSG wolfc :login carlo xxx\r\n");
    purple_debug_info("userserv", "sending %s\n", msg);
    irc_info->send_raw(connection, msg, strlen(msg));
    g_free(msg);
*/
}

static gboolean
plugin_load(PurplePlugin *plugin)
{
    PurplePlugin *prpl;
    void *gc_handle;

    prpl = purple_find_prpl("prpl-irc");
    if(!prpl)
        return FALSE;

    irc_info = PURPLE_PLUGIN_PROTOCOL_INFO(prpl);

    gc_handle = purple_connections_get_handle();

    purple_signal_connect(gc_handle, "signed-on", plugin,
        G_CALLBACK(signed_on), NULL);

    return TRUE;
}

static gboolean
plugin_unload(PurplePlugin *plugin)
{
    return TRUE;
}

static PurplePluginInfo info = {
    PURPLE_PLUGIN_MAGIC,
    PURPLE_MAJOR_VERSION,
    PURPLE_MINOR_VERSION,
    PURPLE_PLUGIN_STANDARD,
    NULL, // ui_requirement
    0, // flags
    NULL, // dependencies
    PURPLE_PRIORITY_DEFAULT,

    PLUGIN_ID,
    "userserv", // name
    VERSION,
    "Makes you logon using userserv.", // summary
    "description",
    "Carlo de Wolf <carlo@nerdnet.nl>",
    NULL, // homepage

    plugin_load,
    plugin_unload,
    NULL, // destroy

    NULL, // ui_info
    NULL, // extra_info
    NULL, // prefs_info

    NULL, // actions

    NULL, // reserved1
    NULL, // reserved2
    NULL, // reserved3
    NULL // reserved4
};

static void
init_plugin(PurplePlugin *plugin)
{
}

PURPLE_INIT_PLUGIN(userserv, init_plugin, info)