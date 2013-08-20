
#include <gst-streaming-server/gss-isom.h>
#include <gst-streaming-server/gss-isom.h>

#include <stdio.h>

#include <openssl/aes.h>


#define GETTEXT_PACKAGE NULL

gboolean verbose;

static GOptionEntry entries[] = {
  {"verbose", 'v', 0, G_OPTION_ARG_NONE, &verbose, "Be verbose", NULL},
  {NULL}
};

int
main (int argc, char *argv[])
{
  GError *error = NULL;
  GOptionContext *context;
  int i;

  context = g_option_context_new ("- ISOM parsing test");
  g_option_context_add_main_entries (context, entries, GETTEXT_PACKAGE);
  g_option_context_add_group (context, gst_init_get_option_group ());
  if (!g_option_context_parse (context, &argc, &argv, &error)) {
    g_print ("option parsing failed: %s", error->message);
    exit (1);
  }
  g_option_context_free (context);

  if (argc < 2) {
    g_print ("need filename\n");
    exit (1);
  }

  for (i = 1; i < argc; i++) {
    GssIsomFile *file;
    GssIsomTrack *video_track;
    GssIsomTrack *audio_track;
    gboolean ret;

    file = gss_isom_file_new ();

    ret = gss_isom_file_parse_file (file, argv[i]);
    if (!ret) {
      g_print ("parse failed");
      continue;
    }

    video_track = gss_isom_movie_get_video_track (file->movie);
    audio_track = gss_isom_movie_get_audio_track (file->movie);
    if (audio_track && video_track) {
      g_print ("%s %d %d\n",
          argv[i], video_track->esds.avg_bitrate,
          audio_track->esds.avg_bitrate);
    } else {
      g_print ("audio %p video %p\n", audio_track, video_track);
    }

    gss_isom_file_free (file);
  }

  return 0;
}
