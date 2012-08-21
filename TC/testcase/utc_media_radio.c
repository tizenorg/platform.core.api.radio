/*
* Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License. 
*/

#include <tet_api.h>
#include <media/radio.h>
#include <glib.h>


enum {
	POSITIVE_TC_IDX = 0x01,
	NEGATIVE_TC_IDX,
};

//keeping ret and radio global, since using startup function for all
radio_h radio;
int sRet;

static void startup(void);
static void cleanup(void);

void (*tet_startup)(void) = startup;
void (*tet_cleanup)(void) = cleanup;

static void utc_media_radio_create_p(void);
static void utc_media_radio_create_n(void);
static void utc_media_radio_destroy_p(void);
static void utc_media_radio_destroy_n(void);
static void utc_media_radio_get_state_p(void);
static void utc_media_radio_get_state_n(void);
static void utc_media_radio_start_p(void);
static void utc_media_radio_start_n(void);
static void utc_media_radio_stop_p(void);
static void utc_media_radio_stop_n(void);
static void utc_media_radio_seek_up_p(void);
static void utc_media_radio_seek_up_n(void);
static void utc_media_radio_seek_down_p(void);
static void utc_media_radio_seek_down_n(void);
static void utc_media_radio_set_frequency_p(void);
static void utc_media_radio_set_frequency_n(void);
static void utc_media_radio_get_frequency_p(void);
static void utc_media_radio_get_frequency_n(void);
static void utc_media_radio_scan_start_p(void);
static void utc_media_radio_scan_start_n(void);
static void utc_media_radio_scan_stop_p(void);
static void utc_media_radio_scan_stop_n(void);
static void utc_media_radio_set_mute_p(void);
static void utc_media_radio_set_mute_n(void);
static void utc_media_radio_is_muted_p(void);
static void utc_media_radio_is_muted_n(void);
static void utc_media_radio_set_scan_completed_callback_p(void);
static void utc_media_radio_set_scan_completed_callback_n(void);
static void utc_media_radio_unset_scan_completed_callback_p(void);
static void utc_media_radio_unset_scan_completed_callback_n(void);
static void utc_media_radio_set_interrupted_callback_p(void);
static void utc_media_radio_set_interrupted_callback_n(void);
static void utc_media_radio_unset_interrupted_callback_p(void);
static void utc_media_radio_unset_interrupted_callback_n(void);

struct tet_testlist tet_testlist[] = {
	{ utc_media_radio_create_p , POSITIVE_TC_IDX },
	{ utc_media_radio_create_n , NEGATIVE_TC_IDX },
	{ utc_media_radio_destroy_p , POSITIVE_TC_IDX },
	{ utc_media_radio_destroy_n , NEGATIVE_TC_IDX },
	{ utc_media_radio_get_state_p , POSITIVE_TC_IDX },
	{ utc_media_radio_get_state_n , NEGATIVE_TC_IDX },
	{ utc_media_radio_start_p , POSITIVE_TC_IDX },
	{ utc_media_radio_start_n  , NEGATIVE_TC_IDX },
	{ utc_media_radio_stop_p , POSITIVE_TC_IDX },
	{ utc_media_radio_stop_n , NEGATIVE_TC_IDX },
	{ utc_media_radio_seek_up_p , POSITIVE_TC_IDX },
	{ utc_media_radio_seek_up_n  , NEGATIVE_TC_IDX },
	{ utc_media_radio_seek_down_p , POSITIVE_TC_IDX },
	{ utc_media_radio_seek_down_n  , NEGATIVE_TC_IDX },
	{ utc_media_radio_set_frequency_p  , POSITIVE_TC_IDX },
	{ utc_media_radio_set_frequency_n , NEGATIVE_TC_IDX },
	{ utc_media_radio_get_frequency_p , POSITIVE_TC_IDX },
	{ utc_media_radio_get_frequency_n  , NEGATIVE_TC_IDX },
	{ utc_media_radio_scan_start_p , POSITIVE_TC_IDX },
	{ utc_media_radio_scan_start_n , NEGATIVE_TC_IDX },
	{ utc_media_radio_scan_stop_p  , POSITIVE_TC_IDX },
	{ utc_media_radio_scan_stop_n , NEGATIVE_TC_IDX },
	{ utc_media_radio_set_mute_p , POSITIVE_TC_IDX },
	{ utc_media_radio_set_mute_n , NEGATIVE_TC_IDX },
	{ utc_media_radio_is_muted_p  , POSITIVE_TC_IDX },
	{ utc_media_radio_is_muted_n, NEGATIVE_TC_IDX },
	{ utc_media_radio_set_scan_completed_callback_p , POSITIVE_TC_IDX },
	{ utc_media_radio_set_scan_completed_callback_n , NEGATIVE_TC_IDX },
	{ utc_media_radio_unset_scan_completed_callback_p , POSITIVE_TC_IDX },
	{ utc_media_radio_unset_scan_completed_callback_n , NEGATIVE_TC_IDX },
	{ utc_media_radio_set_interrupted_callback_p , POSITIVE_TC_IDX },
	{ utc_media_radio_set_interrupted_callback_n , NEGATIVE_TC_IDX },
	{ utc_media_radio_unset_interrupted_callback_p , POSITIVE_TC_IDX },
	{ utc_media_radio_unset_interrupted_callback_n , NEGATIVE_TC_IDX },
	{ NULL, 0 },
};

static GMainLoop *g_mainloop = NULL;
static GThread *event_thread;

gpointer GmainThread(gpointer data){
	g_mainloop = g_main_loop_new (NULL, 0);
	g_main_loop_run (g_mainloop);
	
	return NULL;
}

static void radio_completed_cb(int frequency, void *data)
{
	dts_message("RadiorEvent", "Radio scan info : %d!!!", frequency);
}

static void radio_interrupt_cb(radio_interrupted_code_e code, void *data)
{
	dts_message("RadiorEvent", "Radio Interrupted!!!");
}

static void startup(void)
{
	if( !g_thread_supported() )
	{
		g_thread_init(NULL);
	}
	
	GError *gerr = NULL;
	event_thread = g_thread_create(GmainThread, NULL, 1, &gerr);

	if ((sRet = radio_create(&radio)) != RADIO_ERROR_NONE)
	{
		dts_fail("startup failed");
	}
}

static void cleanup(void)
{
	if ((sRet = radio_destroy(radio)) != RADIO_ERROR_NONE)
	{
		dts_fail("cleanup failed");
	}
	g_main_loop_quit (g_mainloop);
	g_thread_join(event_thread);
}

static void utc_media_radio_create_p(void)
{
	char* api_name = "utc_media_radio_create_p";
	radio_h radio2;
	int ret = radio_create(&radio2);
	if (ret != RADIO_ERROR_NONE)
	{
		dts_message(api_name, "Call log: %d", ret);
		dts_fail(api_name);
	}
	else
	{
		radio_destroy(radio2);
		dts_pass(api_name);
	}
}

static void utc_media_radio_create_n(void)
{
	char* api_name = "utc_media_radio_create_n";
	int ret = radio_create(NULL);
	if (ret == RADIO_ERROR_NONE)
	{
		dts_message(api_name, "Call log: %d", ret);
		dts_fail(api_name);
	}
	else
	{
		dts_pass(api_name);
	}
}

static void utc_media_radio_destroy_p(void)
{
	char* api_name = "utc_media_radio_destroy_p";
	radio_h radio2;
	radio_create(&radio2);
	int ret;
	ret = radio_destroy(radio2);
	if (ret != RADIO_ERROR_NONE)
	{
		dts_message(api_name, "Call log: %d", ret);
		dts_fail(api_name);
	}
	else
	{
		dts_pass(api_name);
	}
}

static void utc_media_radio_destroy_n(void)
{
	char* api_name = "utc_media_radio_destroy_n";
	int ret = radio_destroy(NULL);
	if (ret == RADIO_ERROR_NONE)
	{
		dts_message(api_name, "Call log: %d", ret);
		dts_fail(api_name);
	}
	else
	{
		dts_pass(api_name);
	}
}

static void utc_media_radio_get_state_p(void)
{
	char* api_name = "utc_media_radio_get_state_p";
	int ret;
	radio_state_e state;
	if (sRet == RADIO_ERROR_NONE)
	{
		if ((ret = radio_get_state(radio, &state)) == RADIO_ERROR_NONE)
		{
			dts_pass(api_name);
		}
	}
	dts_message(api_name, "Call log: %d %d", sRet, ret);
	dts_fail(api_name);
}

static void utc_media_radio_get_state_n(void)
{
	char* api_name = "utc_media_radio_get_state_n";
	int ret;
	radio_state_e state;
	if (sRet == RADIO_ERROR_NONE)
	{
		if ((ret = radio_get_state(NULL, &state)) == RADIO_ERROR_INVALID_PARAMETER)
		{
			dts_pass(api_name);
		}
	}
	dts_message(api_name, "Call log: %d %d", sRet, ret);
	dts_fail(api_name);
}

static void utc_media_radio_start_p(void)
{
	char* api_name = "utc_media_radio_start_p";
	int ret;

	if (sRet == RADIO_ERROR_NONE)
	{
		if ((ret = radio_start(radio)) == RADIO_ERROR_NONE)
		{
			radio_stop(radio);
			dts_pass(api_name);
		}
	}
	dts_message(api_name, "Call log: %d %d", sRet, ret);
	dts_fail(api_name);
}

static void utc_media_radio_start_n(void)
{
	char* api_name = "utc_media_radio_start_n";
	int ret;
	if (sRet == RADIO_ERROR_NONE)
	{
		if ((ret = radio_start(NULL)) == RADIO_ERROR_INVALID_PARAMETER)
		{
			dts_pass(api_name);
		}
	}
	dts_message(api_name, "Call log: %d %d", sRet, ret);
	dts_fail(api_name);
}

static void utc_media_radio_stop_p(void)
{
	char* api_name = "utc_media_radio_stop_p";
	int ret;

	if (sRet == RADIO_ERROR_NONE)
	{
		if ((ret = radio_start(radio)) == RADIO_ERROR_NONE)
		{
			if((ret = radio_stop(radio)) == RADIO_ERROR_NONE)
			{
				dts_pass(api_name);
			}
		}
	}
	dts_message(api_name, "Call log: %d %d", sRet, ret);
	dts_fail(api_name);
}

static void utc_media_radio_stop_n(void)
{
	char* api_name = "utc_media_radio_stop_n";
	int ret;
	if (sRet == RADIO_ERROR_NONE)
	{
		if ((ret = radio_stop(NULL)) == RADIO_ERROR_INVALID_PARAMETER)
		{
			dts_pass(api_name);
		}
	}
	dts_message(api_name, "Call log: %d %d", sRet, ret);
	dts_fail(api_name);
}

static void utc_media_radio_seek_up_p(void)
{
	char* api_name = "utc_media_radio_seek_up_p";
	int ret;

	if (sRet == RADIO_ERROR_NONE)
	{
		if ((ret = radio_start(radio)) == RADIO_ERROR_NONE)
		{
			if((ret = radio_seek_up(radio,NULL,NULL)) == RADIO_ERROR_NONE)
			{
				radio_stop(radio);
				dts_pass(api_name);
			}
		}
	}
	dts_message(api_name, "Call log: %d %d", sRet, ret);
	dts_fail(api_name);
}

static void utc_media_radio_seek_up_n(void)
{
	char* api_name = "utc_media_radio_seek_up_n";
	int ret;
	if (sRet == RADIO_ERROR_NONE)
	{
		if ((ret = radio_seek_up(radio,NULL,NULL)) == RADIO_ERROR_INVALID_STATE)
		{
			dts_pass(api_name);
		}
	}
	dts_message(api_name, "Call log: %d %d", sRet, ret);
	dts_fail(api_name);
}

static void utc_media_radio_seek_down_p(void)
{
	char* api_name = "utc_media_radio_seek_down_p";
	int ret;

	if (sRet == RADIO_ERROR_NONE)
	{
		if ((ret = radio_start(radio)) == RADIO_ERROR_NONE)
		{
			if((ret = radio_seek_down(radio,NULL,NULL)) == RADIO_ERROR_NONE)
			{
				radio_stop(radio);
				dts_pass(api_name);
			}
		}
	}
	dts_message(api_name, "Call log: %d %d", sRet, ret);
	dts_fail(api_name);
}

static void utc_media_radio_seek_down_n(void)
{
	char* api_name = "utc_media_radio_seek_down_n";
	int ret;
	if (sRet == RADIO_ERROR_NONE)
	{
		if ((ret = radio_seek_down(radio,NULL,NULL)) == RADIO_ERROR_INVALID_STATE)
		{
			dts_pass(api_name);
		}
	}
	dts_message(api_name, "Call log: %d %d", sRet, ret);
	dts_fail(api_name);
}

static void utc_media_radio_set_frequency_p(void)
{
	char* api_name = "utc_media_radio_set_frequency_p";
	int ret;
	if (sRet == RADIO_ERROR_NONE)
	{
		if ((ret = radio_start(radio)) == RADIO_ERROR_NONE)
		{
			if((ret = radio_set_frequency(radio, 91900)) == RADIO_ERROR_NONE)
			{
				radio_stop(radio);
				dts_pass(api_name);
			}
		}
	}
	dts_message(api_name, "Call log: %d %d", sRet, ret);
	dts_fail(api_name);
}

static void utc_media_radio_set_frequency_n(void)
{
	char* api_name = "utc_media_radio_set_frequency_n";
	int ret;
	if (sRet == RADIO_ERROR_NONE)
	{
		if((ret = radio_set_frequency(radio, 100)) == RADIO_ERROR_INVALID_PARAMETER)
		{
			dts_pass(api_name);
		}
	}
	dts_message(api_name, "Call log: %d %d", sRet, ret);
	dts_fail(api_name);
}

static void utc_media_radio_get_frequency_p(void)
{
	char* api_name = "utc_media_radio_get_frequency_p";
	int ret;
	if (sRet == RADIO_ERROR_NONE)
	{
		if ((ret = radio_start(radio)) == RADIO_ERROR_NONE)
		{
			int frequency;
			if((ret = radio_get_frequency(radio, &frequency)) == RADIO_ERROR_NONE)
			{
				radio_stop(radio);
				dts_pass(api_name);
			}
		}
	}
	dts_message(api_name, "Call log: %d %d", sRet, ret);
	dts_fail(api_name);
}

static void utc_media_radio_get_frequency_n(void)
{
	char* api_name = "utc_media_radio_get_frequency_n";
	int ret;
	if (sRet == RADIO_ERROR_NONE)
	{
		if((ret = radio_get_frequency(NULL, NULL)) == RADIO_ERROR_INVALID_PARAMETER)
		{
			dts_pass(api_name);
		}
	}
	dts_message(api_name, "Call log: %d %d", sRet, ret);
	dts_fail(api_name);
}

static void utc_media_radio_scan_start_p(void)
{
	char* api_name = "utc_media_radio_scan_start_p";
	int ret;
	if (sRet == RADIO_ERROR_NONE)
	{
		if ((ret = radio_scan_start(radio,NULL,NULL)) == RADIO_ERROR_NONE)
		{
			radio_scan_stop(radio,NULL,NULL);
			dts_pass(api_name);
		}
	}
	dts_message(api_name, "Call log: %d %d", sRet, ret);
	dts_fail(api_name);
}

static void utc_media_radio_scan_start_n(void)
{
	char* api_name = "utc_media_radio_scan_start_n";
	int ret;
	if (sRet == RADIO_ERROR_NONE)
	{
		if ((ret = radio_scan_start(NULL,NULL,NULL)) == RADIO_ERROR_INVALID_PARAMETER)
		{
			dts_pass(api_name);
		}
	}
	dts_message(api_name, "Call log: %d %d", sRet, ret);
	dts_fail(api_name);
}

static void utc_media_radio_scan_stop_p(void)
{
	char* api_name = "utc_media_radio_scan_stop_p";
	int ret;
	
	if (sRet == RADIO_ERROR_NONE)
	{
		if ((ret = radio_scan_start(radio,NULL,NULL)) == RADIO_ERROR_NONE)
		{
			if((ret = radio_scan_stop(radio,NULL,NULL)) == RADIO_ERROR_NONE)
			{
				dts_pass(api_name);
			}
		}
	}
	dts_message(api_name, "Call log: %d %d", sRet, ret);
	dts_fail(api_name);
}

static void utc_media_radio_scan_stop_n(void)
{
	char* api_name = "utc_media_radio_scan_stop_n";
	int ret;
	if (sRet == RADIO_ERROR_NONE)
	{
		if ((ret = radio_scan_stop(NULL,NULL,NULL)) == RADIO_ERROR_INVALID_PARAMETER)
		{
			dts_pass(api_name);
		}
	}
	dts_message(api_name, "Call log: %d %d", sRet, ret);
	dts_fail(api_name);
}

static void utc_media_radio_set_mute_p(void)
{
	char* api_name = "utc_media_radio_set_mute_p";
	int ret;
	bool mute = TRUE;
	if (sRet == RADIO_ERROR_NONE)
	{
		if ((ret = radio_set_mute(radio, mute)) == RADIO_ERROR_NONE)
		{
			dts_pass(api_name);
		}
	}
	dts_message(api_name, "Call log: %d %d", sRet, ret);
	dts_fail(api_name);
}

static void utc_media_radio_set_mute_n(void)
{
	char* api_name = "utc_media_radio_set_mute_n";
	int ret;
	if (sRet == RADIO_ERROR_NONE)
	{
		if ((ret = radio_set_mute(NULL,TRUE)) == RADIO_ERROR_INVALID_PARAMETER)
		{
			dts_pass(api_name);
		}
	}
	dts_message(api_name, "Call log: %d %d", sRet, ret);
	dts_fail(api_name);
}

static void utc_media_radio_is_muted_p(void)
{
	char* api_name = "utc_media_radio_is_muted_p";
	int ret;
	bool mute;
	if (sRet == RADIO_ERROR_NONE)
	{
		if ((ret = radio_is_muted(radio,&mute)) == RADIO_ERROR_NONE)
		{
			dts_pass(api_name);
		}
	}
	dts_message(api_name, "Call log: %d %d", sRet, ret);
	dts_fail(api_name);
}

static void utc_media_radio_is_muted_n(void)
{
	char* api_name = "utc_media_radio_is_muted_n";
	int ret;
	if (sRet == RADIO_ERROR_NONE)
	{
		if ((ret = radio_is_muted(NULL,NULL)) == RADIO_ERROR_INVALID_PARAMETER)
		{
			dts_pass(api_name);
		}
	}
	dts_message(api_name, "Call log: %d %d", sRet, ret);
	dts_fail(api_name);
}

static void utc_media_radio_set_scan_completed_callback_p(void)
{
	char* api_name = "utc_media_radio_set_scan_completed_callback_p";
	int ret;
	if (sRet == RADIO_ERROR_NONE)
	{
		if ((ret = radio_set_scan_completed_cb(radio, radio_completed_cb, NULL)) == RADIO_ERROR_NONE)
		{
			dts_pass(api_name);
		}
	}
	dts_message(api_name, "Call log: %d %d", sRet, ret);
	dts_fail(api_name);
}

static void utc_media_radio_set_scan_completed_callback_n(void)
{
	char* api_name = "utc_media_radio_set_scan_completed_callback_n";
	int ret;
	if (sRet == RADIO_ERROR_NONE)
	{
		if ((ret = radio_set_scan_completed_cb(radio, NULL, NULL)) == RADIO_ERROR_INVALID_PARAMETER)
		{
			dts_pass(api_name);
		}
	}
	dts_message(api_name, "Call log: %d %d", sRet, ret);
	dts_fail(api_name);
}

static void utc_media_radio_unset_scan_completed_callback_p(void)
{
	char* api_name = "utc_media_radio_unset_scan_completed_callback_p";
	int ret;
	if (sRet == RADIO_ERROR_NONE)
	{
		if ((ret = radio_unset_scan_completed_cb(radio)) == RADIO_ERROR_NONE)
		{
			dts_pass(api_name);
		}
	}
	dts_message(api_name, "Call log: %d %d", sRet, ret);
	dts_fail(api_name);
}

static void utc_media_radio_unset_scan_completed_callback_n(void)
{
	char* api_name = "utc_media_radio_unset_scan_completed_callback_n";
	int ret;
	if (sRet == RADIO_ERROR_NONE)
	{
		if ((ret = radio_unset_scan_completed_cb(NULL)) == RADIO_ERROR_INVALID_PARAMETER)
		{
			dts_pass(api_name);
		}
	}
	dts_message(api_name, "Call log: %d %d", sRet, ret);
	dts_fail(api_name);
}

static void utc_media_radio_set_interrupted_callback_p(void)
{
	char* api_name = "utc_media_radio_set_interrupted_callback_p";
	int ret;
	if (sRet == RADIO_ERROR_NONE)
	{
		if ((ret = radio_set_interrupted_cb(radio, radio_interrupt_cb, NULL)) == RADIO_ERROR_NONE)
		{
			dts_pass(api_name);
		}
	}
	dts_message(api_name, "Call log: %d %d", sRet, ret);
	dts_fail(api_name);
}

static void utc_media_radio_set_interrupted_callback_n(void)
{
	char* api_name = "utc_media_radio_set_interrupted_callback_n";
	int ret;
	if (sRet == RADIO_ERROR_NONE)
	{
		if ((ret = radio_set_interrupted_cb(radio, NULL, NULL)) == RADIO_ERROR_INVALID_PARAMETER)
		{
			dts_pass(api_name);
		}
	}
	dts_message(api_name, "Call log: %d %d", sRet, ret);
	dts_fail(api_name);
}

static void utc_media_radio_unset_interrupted_callback_p(void)
{
	char* api_name = "utc_media_radio_unset_interrupted_callback_p";
	int ret;
	if (sRet == RADIO_ERROR_NONE)
	{
		if ((ret = radio_unset_interrupted_cb(radio)) == RADIO_ERROR_NONE)
		{
			dts_pass(api_name);
		}
	}
	dts_message(api_name, "Call log: %d %d", sRet, ret);
	dts_fail(api_name);
}

static void utc_media_radio_unset_interrupted_callback_n(void)
{
	char* api_name = "utc_media_radio_unset_interrupted_callback_n";
	int ret;
	if (sRet == RADIO_ERROR_NONE)
	{
		if ((ret = radio_unset_interrupted_cb(NULL)) == RADIO_ERROR_INVALID_PARAMETER)
		{
			dts_pass(api_name);
		}
	}
	dts_message(api_name, "Call log: %d %d", sRet, ret);
	dts_fail(api_name);
}

