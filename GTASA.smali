.class public Lcom/rockstargames/gtasa/GTASA;
.super Lcom/wardrumstudios/utils/WarMedia;
.source "GTASA.java"


# static fields
.field static UseAndroidHal:Z

.field private static debugStaticCheck:I

.field public static gtasaSelf:Lcom/rockstargames/gtasa/GTASA;

.field static vmVersion:Ljava/lang/String;


# instance fields
.field IsAmazonBuild:Z

.field IsGermanBuild:Z

.field final UseADX:Z

.field UseExpansionPack:Z

.field final hapt_ids:[I

.field final hapt_names:[Ljava/lang/String;

.field public isInSocialClub:Z


# direct methods
.method static constructor <clinit>()V
    .locals 3

    .prologue
    const/4 v1, 0x0

    .line 38
    const/4 v0, 0x1

    sput-boolean v0, Lcom/rockstargames/gtasa/GTASA;->UseAndroidHal:Z

    .line 45
    const/4 v0, 0x0

    sput v0, Lcom/rockstargames/gtasa/GTASA;->debugStaticCheck:I

    .line 47
    sput-object v1, Lcom/rockstargames/gtasa/GTASA;->gtasaSelf:Lcom/rockstargames/gtasa/GTASA;

    .line 52
    sput-object v1, Lcom/rockstargames/gtasa/GTASA;->vmVersion:Ljava/lang/String;

    .line 448
    sget-object v0, Ljava/lang/System;->out:Ljava/io/PrintStream;

    const-string v1, "**** Loading SO\'s"

    invoke-virtual {v0, v1}, Ljava/io/PrintStream;->println(Ljava/lang/String;)V

    .line 450
    :try_start_0
    const-string v0, "java.vm.version"

    invoke-static {v0}, Ljava/lang/System;->getProperty(Ljava/lang/String;)Ljava/lang/String;

    move-result-object v0

    sput-object v0, Lcom/rockstargames/gtasa/GTASA;->vmVersion:Ljava/lang/String;

    .line 451
    sget-object v0, Ljava/lang/System;->out:Ljava/io/PrintStream;

    new-instance v1, Ljava/lang/StringBuilder;

    invoke-direct {v1}, Ljava/lang/StringBuilder;-><init>()V

    const-string v2, "vmVersion "

    invoke-virtual {v1, v2}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v1

    sget-object v2, Lcom/rockstargames/gtasa/GTASA;->vmVersion:Ljava/lang/String;

    invoke-virtual {v1, v2}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v1

    invoke-virtual {v1}, Ljava/lang/StringBuilder;->toString()Ljava/lang/String;

    move-result-object v1

    invoke-virtual {v0, v1}, Ljava/io/PrintStream;->println(Ljava/lang/String;)V

    .line 452
    const-string v0, "ImmEmulatorJ"

    invoke-static {v0}, Ljava/lang/System;->loadLibrary(Ljava/lang/String;)V
    :try_end_0
    .catch Ljava/lang/UnsatisfiedLinkError; {:try_start_0 .. :try_end_0} :catch_1
    .catch Ljava/lang/ExceptionInInitializerError; {:try_start_0 .. :try_end_0} :catch_0

    .line 455
    :goto_0
    const-string v0, "SCAnd"

    invoke-static {v0}, Ljava/lang/System;->loadLibrary(Ljava/lang/String;)V

    .line 456
    const-string v0, "GTASA"

    invoke-static {v0}, Ljava/lang/System;->loadLibrary(Ljava/lang/String;)V

    .line 457
    const-string v0, "mtm"

    invoke-static {v0}, Ljava/lang/System;->loadLibrary(Ljava/lang/String;)V

    .line 458
    return-void

    .line 454
    :catch_0
    move-exception v0

    goto :goto_0

    .line 453
    :catch_1
    move-exception v0

    goto :goto_0
.end method

.method public constructor <init>()V
    .locals 5

    .prologue
    const/16 v4, 0x1e

    const/4 v3, 0x0

    const/4 v2, 0x1

    .line 35
    invoke-direct {p0}, Lcom/wardrumstudios/utils/WarMedia;-><init>()V

    .line 42
    iput-boolean v2, p0, Lcom/rockstargames/gtasa/GTASA;->UseExpansionPack:Z

    .line 43
    iput-boolean v3, p0, Lcom/rockstargames/gtasa/GTASA;->IsAmazonBuild:Z

    .line 48
    iput-boolean v2, p0, Lcom/rockstargames/gtasa/GTASA;->isInSocialClub:Z

    .line 50
    iput-boolean v3, p0, Lcom/rockstargames/gtasa/GTASA;->IsGermanBuild:Z

    .line 54
    iput-boolean v2, p0, Lcom/rockstargames/gtasa/GTASA;->UseADX:Z

    .line 146
    new-array v0, v4, [Ljava/lang/String;

    const-string v1, "bounce.hapt"

    aput-object v1, v0, v3

    const-string v1, "bounce_light.hapt"

    aput-object v1, v0, v2

    const/4 v1, 0x2

    const-string v2, "bump.hapt"

    aput-object v2, v0, v1

    const/4 v1, 0x3

    const-string v2, "bump_light.hapt"

    aput-object v2, v0, v1

    const/4 v1, 0x4

    const-string v2, "bumpy_texture.hapt"

    aput-object v2, v0, v1

    const/4 v1, 0x5

    const-string v2, "crunchy_texture.hapt"

    aput-object v2, v0, v1

    const/4 v1, 0x6

    const-string v2, "double_click.hapt"

    aput-object v2, v0, v1

    const/4 v1, 0x7

    const-string v2, "double_click_light.hapt"

    aput-object v2, v0, v1

    const/16 v1, 0x8

    const-string v2, "engine.hapt"

    aput-object v2, v0, v1

    const/16 v1, 0x9

    const-string v2, "engine_light.hapt"

    aput-object v2, v0, v1

    const/16 v1, 0xa

    const-string v2, "fast_pulse.hapt"

    aput-object v2, v0, v1

    const/16 v1, 0xb

    const-string v2, "heartbeat.hapt"

    aput-object v2, v0, v1

    const/16 v1, 0xc

    const-string v2, "long_explosion.hapt"

    aput-object v2, v0, v1

    const/16 v1, 0xd

    const-string v2, "long_ramp_down.hapt"

    aput-object v2, v0, v1

    const/16 v1, 0xe

    const-string v2, "long_ramp_down_light.hapt"

    aput-object v2, v0, v1

    const/16 v1, 0xf

    const-string v2, "long_ramp_up.hapt"

    aput-object v2, v0, v1

    const/16 v1, 0x10

    const-string v2, "long_ramp_up_light.hapt"

    aput-object v2, v0, v1

    const/16 v1, 0x11

    const-string v2, "machine_gun.hapt"

    aput-object v2, v0, v1

    const/16 v1, 0x12

    const-string v2, "pop.hapt"

    aput-object v2, v0, v1

    const/16 v1, 0x13

    const-string v2, "pop_light.hapt"

    aput-object v2, v0, v1

    const/16 v1, 0x14

    const-string v2, "short_explosion.hapt"

    aput-object v2, v0, v1

    const/16 v1, 0x15

    const-string v2, "short_ramp_down.hapt"

    aput-object v2, v0, v1

    const/16 v1, 0x16

    const-string v2, "short_ramp_down_light.hapt"

    aput-object v2, v0, v1

    const/16 v1, 0x17

    const-string v2, "short_ramp_up.hapt"

    aput-object v2, v0, v1

    const/16 v1, 0x18

    const-string v2, "short_ramp_up_light.hapt"

    aput-object v2, v0, v1

    const/16 v1, 0x19

    const-string v2, "slow_pulse.hapt"

    aput-object v2, v0, v1

    const/16 v1, 0x1a

    const-string v2, "tick.hapt"

    aput-object v2, v0, v1

    const/16 v1, 0x1b

    const-string v2, "tick_light.hapt"

    aput-object v2, v0, v1

    const/16 v1, 0x1c

    const-string v2, "triple_click.hapt"

    aput-object v2, v0, v1

    const/16 v1, 0x1d

    const-string v2, "triple_click_light.hapt"

    aput-object v2, v0, v1

    iput-object v0, p0, Lcom/rockstargames/gtasa/GTASA;->hapt_names:[Ljava/lang/String;

    .line 147
    new-array v0, v4, [I

    fill-array-data v0, :array_0

    iput-object v0, p0, Lcom/rockstargames/gtasa/GTASA;->hapt_ids:[I

    return-void

    nop

    :array_0
    .array-data 4
        0x7f040000
        0x7f040001
        0x7f040002
        0x7f040003
        0x7f040004
        0x7f040005
        0x7f040006
        0x7f040007
        0x7f040008
        0x7f040009
        0x7f04000a
        0x7f04000b
        0x7f04000c
        0x7f04000d
        0x7f04000e
        0x7f04000f
        0x7f040010
        0x7f040011
        0x7f040012
        0x7f040013
        0x7f040014
        0x7f040015
        0x7f040016
        0x7f040017
        0x7f040018
        0x7f040019
        0x7f04001a
        0x7f04001b
        0x7f04001c
        0x7f04001d
    .end array-data
.end method

.method private copyHaptsToInternalStorage([Ljava/lang/String;)Ljava/lang/String;
    .locals 14
    .param p1, "haptFileName"    # [Ljava/lang/String;

    .prologue
    .line 149
    const/4 v4, 0x0

    .line 150
    .local v4, "fos":Ljava/io/FileOutputStream;
    const/16 v8, 0x400

    .local v8, "maxLen":I
    const/4 v0, 0x0

    .line 151
    .local v0, "curLen":I
    new-array v10, v8, [B

    .line 153
    .local v10, "tempBuffer":[B
    const/4 v5, 0x0

    .local v5, "i":I
    :goto_0
    const/16 v11, 0x1e

    if-ge v5, v11, :cond_1

    .line 154
    invoke-virtual {p0}, Lcom/rockstargames/gtasa/GTASA;->getResources()Landroid/content/res/Resources;

    move-result-object v11

    iget-object v12, p0, Lcom/rockstargames/gtasa/GTASA;->hapt_ids:[I

    aget v12, v12, v5

    invoke-virtual {v11, v12}, Landroid/content/res/Resources;->openRawResource(I)Ljava/io/InputStream;

    move-result-object v7

    .line 156
    .local v7, "is":Ljava/io/InputStream;
    :try_start_0
    aget-object v11, p1, v5

    const/4 v12, 0x0

    invoke-virtual {p0, v11, v12}, Lcom/rockstargames/gtasa/GTASA;->openFileOutput(Ljava/lang/String;I)Ljava/io/FileOutputStream;

    move-result-object v4

    .line 157
    invoke-virtual {v7}, Ljava/io/InputStream;->available()I

    move-result v0

    .line 158
    :goto_1
    if-lez v0, :cond_0

    .line 160
    invoke-virtual {v7, v10}, Ljava/io/InputStream;->read([B)I

    move-result v0

    .line 161
    const/4 v11, 0x0

    invoke-virtual {v4, v10, v11, v0}, Ljava/io/FileOutputStream;->write([BII)V

    .line 162
    invoke-virtual {v7}, Ljava/io/InputStream;->available()I

    move-result v0

    goto :goto_1

    .line 164
    :cond_0
    invoke-virtual {v4}, Ljava/io/FileOutputStream;->close()V

    .line 165
    invoke-virtual {v7}, Ljava/io/InputStream;->close()V
    :try_end_0
    .catch Ljava/io/FileNotFoundException; {:try_start_0 .. :try_end_0} :catch_1
    .catch Ljava/io/IOException; {:try_start_0 .. :try_end_0} :catch_3
    .catchall {:try_start_0 .. :try_end_0} :catchall_0

    .line 172
    :try_start_1
    invoke-virtual {v4}, Ljava/io/FileOutputStream;->close()V

    .line 173
    invoke-virtual {v7}, Ljava/io/InputStream;->close()V
    :try_end_1
    .catch Ljava/io/IOException; {:try_start_1 .. :try_end_1} :catch_0

    .line 153
    :goto_2
    add-int/lit8 v5, v5, 0x1

    goto :goto_0

    .line 174
    :catch_0
    move-exception v1

    .line 175
    .local v1, "e":Ljava/io/IOException;
    invoke-virtual {v1}, Ljava/io/IOException;->printStackTrace()V

    goto :goto_2

    .line 166
    .end local v1    # "e":Ljava/io/IOException;
    :catch_1
    move-exception v3

    .line 167
    .local v3, "fnfe":Ljava/io/FileNotFoundException;
    :try_start_2
    const-string v11, "OSWrapper"

    new-instance v12, Ljava/lang/StringBuilder;

    invoke-direct {v12}, Ljava/lang/StringBuilder;-><init>()V

    const-string v13, "Haptic Files not found\n"

    invoke-virtual {v12, v13}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v12

    invoke-virtual {v12, v3}, Ljava/lang/StringBuilder;->append(Ljava/lang/Object;)Ljava/lang/StringBuilder;

    move-result-object v12

    invoke-virtual {v12}, Ljava/lang/StringBuilder;->toString()Ljava/lang/String;

    move-result-object v12

    invoke-static {v11, v12}, Landroid/util/Log;->e(Ljava/lang/String;Ljava/lang/String;)I
    :try_end_2
    .catchall {:try_start_2 .. :try_end_2} :catchall_0

    .line 172
    :try_start_3
    invoke-virtual {v4}, Ljava/io/FileOutputStream;->close()V

    .line 173
    invoke-virtual {v7}, Ljava/io/InputStream;->close()V
    :try_end_3
    .catch Ljava/io/IOException; {:try_start_3 .. :try_end_3} :catch_2

    goto :goto_2

    .line 174
    :catch_2
    move-exception v1

    .line 175
    .restart local v1    # "e":Ljava/io/IOException;
    invoke-virtual {v1}, Ljava/io/IOException;->printStackTrace()V

    goto :goto_2

    .line 168
    .end local v1    # "e":Ljava/io/IOException;
    .end local v3    # "fnfe":Ljava/io/FileNotFoundException;
    :catch_3
    move-exception v6

    .line 169
    .local v6, "ioe":Ljava/io/IOException;
    :try_start_4
    const-string v11, "OSWrapper"

    new-instance v12, Ljava/lang/StringBuilder;

    invoke-direct {v12}, Ljava/lang/StringBuilder;-><init>()V

    const-string v13, "IOException when loading .hapts."

    invoke-virtual {v12, v13}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v12

    invoke-virtual {v12, v6}, Ljava/lang/StringBuilder;->append(Ljava/lang/Object;)Ljava/lang/StringBuilder;

    move-result-object v12

    invoke-virtual {v12}, Ljava/lang/StringBuilder;->toString()Ljava/lang/String;

    move-result-object v12

    invoke-static {v11, v12}, Landroid/util/Log;->e(Ljava/lang/String;Ljava/lang/String;)I
    :try_end_4
    .catchall {:try_start_4 .. :try_end_4} :catchall_0

    .line 172
    :try_start_5
    invoke-virtual {v4}, Ljava/io/FileOutputStream;->close()V

    .line 173
    invoke-virtual {v7}, Ljava/io/InputStream;->close()V
    :try_end_5
    .catch Ljava/io/IOException; {:try_start_5 .. :try_end_5} :catch_4

    goto :goto_2

    .line 174
    :catch_4
    move-exception v1

    .line 175
    .restart local v1    # "e":Ljava/io/IOException;
    invoke-virtual {v1}, Ljava/io/IOException;->printStackTrace()V

    goto :goto_2

    .line 171
    .end local v1    # "e":Ljava/io/IOException;
    .end local v6    # "ioe":Ljava/io/IOException;
    :catchall_0
    move-exception v11

    .line 172
    :try_start_6
    invoke-virtual {v4}, Ljava/io/FileOutputStream;->close()V

    .line 173
    invoke-virtual {v7}, Ljava/io/InputStream;->close()V
    :try_end_6
    .catch Ljava/io/IOException; {:try_start_6 .. :try_end_6} :catch_5

    .line 176
    :goto_3
    throw v11

    .line 174
    :catch_5
    move-exception v1

    .line 175
    .restart local v1    # "e":Ljava/io/IOException;
    invoke-virtual {v1}, Ljava/io/IOException;->printStackTrace()V

    goto :goto_3

    .line 181
    .end local v1    # "e":Ljava/io/IOException;
    .end local v7    # "is":Ljava/io/InputStream;
    :cond_1
    invoke-virtual {p0}, Lcom/rockstargames/gtasa/GTASA;->getFilesDir()Ljava/io/File;

    move-result-object v2

    .line 182
    .local v2, "file":Ljava/io/File;
    new-instance v11, Ljava/lang/StringBuilder;

    invoke-direct {v11}, Ljava/lang/StringBuilder;-><init>()V

    invoke-virtual {v2}, Ljava/io/File;->getAbsolutePath()Ljava/lang/String;

    move-result-object v12

    invoke-virtual {v11, v12}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v11

    sget-object v12, Ljava/io/File;->separator:Ljava/lang/String;

    invoke-virtual {v11, v12}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v11

    invoke-virtual {v11}, Ljava/lang/StringBuilder;->toString()Ljava/lang/String;

    move-result-object v9

    .line 183
    .local v9, "path":Ljava/lang/String;
    return-object v9
.end method

.method public static staticEnterSocialClub()V
    .locals 1

    .prologue
    .line 436
    sget-object v0, Lcom/rockstargames/gtasa/GTASA;->gtasaSelf:Lcom/rockstargames/gtasa/GTASA;

    invoke-virtual {v0}, Lcom/rockstargames/gtasa/GTASA;->EnterSocialClub()V

    .line 437
    return-void
.end method

.method public static staticExitSocialClub()V
    .locals 1

    .prologue
    .line 432
    sget-object v0, Lcom/rockstargames/gtasa/GTASA;->gtasaSelf:Lcom/rockstargames/gtasa/GTASA;

    invoke-virtual {v0}, Lcom/rockstargames/gtasa/GTASA;->ExitSocialClub()V

    .line 433
    return-void
.end method


# virtual methods
.method public AfterDownloadFunction()V
    .locals 7

    .prologue
    const/4 v2, 0x1

    const/4 v3, 0x0

    .line 214
    :try_start_0
    sget-boolean v4, Lcom/rockstargames/gtasa/GTASA;->UseAndroidHal:Z

    if-eqz v4, :cond_0

    .line 216
    invoke-virtual {p0}, Lcom/rockstargames/gtasa/GTASA;->getWindow()Landroid/view/Window;

    move-result-object v4

    const/16 v5, 0x400

    const/16 v6, 0x400

    invoke-virtual {v4, v5, v6}, Landroid/view/Window;->setFlags(II)V

    .line 218
    invoke-static {p0}, Lcom/rockstargames/hal/ActivityWrapper;->setActivity(Landroid/app/Activity;)V

    .line 219
    invoke-static {}, Lcom/rockstargames/hal/ActivityWrapper;->getTransitioning()Z

    move-result v4

    if-eqz v4, :cond_1

    .line 221
    const-string v4, "StartupActivity"

    new-instance v5, Ljava/lang/StringBuilder;

    invoke-direct {v5}, Ljava/lang/StringBuilder;-><init>()V

    const-string v6, "*** MainActivity onCreate() transitioning ("

    invoke-virtual {v5, v6}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v5

    sget v6, Lcom/rockstargames/gtasa/GTASA;->debugStaticCheck:I

    invoke-virtual {v5, v6}, Ljava/lang/StringBuilder;->append(I)Ljava/lang/StringBuilder;

    move-result-object v5

    const-string v6, ") ***)"

    invoke-virtual {v5, v6}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v5

    invoke-virtual {v5}, Ljava/lang/StringBuilder;->toString()Ljava/lang/String;

    move-result-object v5

    invoke-static {v4, v5}, Landroid/util/Log;->i(Ljava/lang/String;Ljava/lang/String;)I

    .line 222
    const/4 v4, 0x0

    invoke-static {v4}, Lcom/rockstargames/hal/ActivityWrapper;->setTransitioning(Z)V

    .line 227
    :goto_0
    sget v4, Lcom/rockstargames/gtasa/GTASA;->debugStaticCheck:I

    add-int/lit8 v4, v4, 0x1

    sput v4, Lcom/rockstargames/gtasa/GTASA;->debugStaticCheck:I

    .line 229
    invoke-virtual {p0}, Lcom/rockstargames/gtasa/GTASA;->getWindowManager()Landroid/view/WindowManager;

    move-result-object v4

    invoke-interface {v4}, Landroid/view/WindowManager;->getDefaultDisplay()Landroid/view/Display;

    move-result-object v0

    .line 231
    .local v0, "d":Landroid/view/Display;
    invoke-virtual {p0}, Lcom/rockstargames/gtasa/GTASA;->getResources()Landroid/content/res/Resources;

    move-result-object v4

    invoke-virtual {v4}, Landroid/content/res/Resources;->getConfiguration()Landroid/content/res/Configuration;

    move-result-object v4

    iget v4, v4, Landroid/content/res/Configuration;->orientation:I

    if-ne v4, v2, :cond_2

    move v4, v2

    :goto_1
    invoke-virtual {v0}, Landroid/view/Display;->getWidth()I

    move-result v5

    invoke-virtual {v0}, Landroid/view/Display;->getHeight()I

    move-result v6

    if-ge v5, v6, :cond_3

    :goto_2
    if-ne v4, v2, :cond_4

    .line 232
    invoke-virtual {v0}, Landroid/view/Display;->getWidth()I

    move-result v2

    invoke-virtual {v0}, Landroid/view/Display;->getHeight()I

    move-result v3

    invoke-static {v2, v3}, Lcom/rockstargames/hal/ActivityWrapper;->runMain(II)V

    .line 242
    .end local v0    # "d":Landroid/view/Display;
    :cond_0
    :goto_3
    invoke-virtual {p0}, Lcom/rockstargames/gtasa/GTASA;->DoResumeEvent()V

    .line 248
    :goto_4
    return-void

    .line 225
    :cond_1
    const-string v4, "StartupActivity"

    new-instance v5, Ljava/lang/StringBuilder;

    invoke-direct {v5}, Ljava/lang/StringBuilder;-><init>()V

    const-string v6, "*** MainActivity onCreate() not transitioning ("

    invoke-virtual {v5, v6}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v5

    sget v6, Lcom/rockstargames/gtasa/GTASA;->debugStaticCheck:I

    invoke-virtual {v5, v6}, Ljava/lang/StringBuilder;->append(I)Ljava/lang/StringBuilder;

    move-result-object v5

    const-string v6, ") ***)"

    invoke-virtual {v5, v6}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v5

    invoke-virtual {v5}, Ljava/lang/StringBuilder;->toString()Ljava/lang/String;

    move-result-object v5

    invoke-static {v4, v5}, Landroid/util/Log;->i(Ljava/lang/String;Ljava/lang/String;)I
    :try_end_0
    .catch Ljava/lang/Exception; {:try_start_0 .. :try_end_0} :catch_0

    goto :goto_0

    .line 244
    :catch_0
    move-exception v1

    .line 246
    .local v1, "ex":Ljava/lang/Exception;
    new-instance v2, Landroid/app/AlertDialog$Builder;

    invoke-direct {v2, p0}, Landroid/app/AlertDialog$Builder;-><init>(Landroid/content/Context;)V

    const-string v3, "Exception"

    invoke-virtual {v2, v3}, Landroid/app/AlertDialog$Builder;->setTitle(Ljava/lang/CharSequence;)Landroid/app/AlertDialog$Builder;

    move-result-object v2

    invoke-virtual {v1}, Ljava/lang/Exception;->getLocalizedMessage()Ljava/lang/String;

    move-result-object v3

    invoke-virtual {v2, v3}, Landroid/app/AlertDialog$Builder;->setMessage(Ljava/lang/CharSequence;)Landroid/app/AlertDialog$Builder;

    move-result-object v2

    invoke-virtual {v2}, Landroid/app/AlertDialog$Builder;->show()Landroid/app/AlertDialog;

    goto :goto_4

    .end local v1    # "ex":Ljava/lang/Exception;
    .restart local v0    # "d":Landroid/view/Display;
    :cond_2
    move v4, v3

    .line 231
    goto :goto_1

    :cond_3
    move v2, v3

    goto :goto_2

    .line 234
    :cond_4
    :try_start_1
    invoke-virtual {v0}, Landroid/view/Display;->getHeight()I

    move-result v2

    invoke-virtual {v0}, Landroid/view/Display;->getWidth()I

    move-result v3

    invoke-static {v2, v3}, Lcom/rockstargames/hal/ActivityWrapper;->runMain(II)V
    :try_end_1
    .catch Ljava/lang/Exception; {:try_start_1 .. :try_end_1} :catch_0

    goto :goto_3
.end method

.method public CustomLoadFunction()Z
    .locals 1

    .prologue
    .line 441
    sget-object v0, Lcom/rockstargames/gtasa/GTASA;->gtasaSelf:Lcom/rockstargames/gtasa/GTASA;

    invoke-virtual {p0, v0}, Lcom/rockstargames/gtasa/GTASA;->CheckIfNeedsReadPermission(Landroid/app/Activity;)Z

    move-result v0

    return v0
.end method

.method public EnterSocialClub()V
    .locals 1

    .prologue
    .line 401
    new-instance v0, Lcom/rockstargames/gtasa/GTASA$1;

    invoke-direct {v0, p0}, Lcom/rockstargames/gtasa/GTASA$1;-><init>(Lcom/rockstargames/gtasa/GTASA;)V

    invoke-virtual {p0, v0}, Lcom/rockstargames/gtasa/GTASA;->runOnUiThread(Ljava/lang/Runnable;)V

    .line 418
    return-void
.end method

.method public ExitSocialClub()V
    .locals 1

    .prologue
    .line 421
    const/4 v0, 0x0

    iput-boolean v0, p0, Lcom/rockstargames/gtasa/GTASA;->isInSocialClub:Z

    .line 422
    iget-object v0, p0, Lcom/rockstargames/gtasa/GTASA;->view:Landroid/view/SurfaceView;

    invoke-virtual {v0}, Landroid/view/SurfaceView;->getParent()Landroid/view/ViewParent;

    move-result-object v0

    if-eqz v0, :cond_0

    .line 423
    iget-object v0, p0, Lcom/rockstargames/gtasa/GTASA;->view:Landroid/view/SurfaceView;

    invoke-virtual {v0}, Landroid/view/SurfaceView;->getParent()Landroid/view/ViewParent;

    move-result-object v0

    check-cast v0, Landroid/view/View;

    invoke-virtual {p0, v0}, Lcom/rockstargames/gtasa/GTASA;->setContentView(Landroid/view/View;)V

    .line 427
    :goto_0
    const/4 v0, 0x6

    invoke-virtual {p0, v0}, Lcom/rockstargames/gtasa/GTASA;->setRequestedOrientation(I)V

    .line 428
    return-void

    .line 426
    :cond_0
    iget-object v0, p0, Lcom/rockstargames/gtasa/GTASA;->view:Landroid/view/SurfaceView;

    invoke-virtual {p0, v0}, Lcom/rockstargames/gtasa/GTASA;->setContentView(Landroid/view/View;)V

    goto :goto_0
.end method

.method public ServiceAppCommand(Ljava/lang/String;Ljava/lang/String;)Z
    .locals 10
    .param p1, "cmd"    # Ljava/lang/String;
    .param p2, "args"    # Ljava/lang/String;

    .prologue
    const/4 v4, 0x1

    const/4 v3, 0x0

    .line 253
    const-string v5, "ForceGermanBuild"

    invoke-virtual {p1, v5}, Ljava/lang/String;->equalsIgnoreCase(Ljava/lang/String;)Z

    move-result v5

    if-eqz v5, :cond_1

    .line 254
    iget-boolean v3, p0, Lcom/rockstargames/gtasa/GTASA;->IsGermanBuild:Z

    .line 286
    :cond_0
    :goto_0
    return v3

    .line 256
    :cond_1
    const-string v5, "SetLocale"

    invoke-virtual {p1, v5}, Ljava/lang/String;->equalsIgnoreCase(Ljava/lang/String;)Z

    move-result v5

    if-eqz v5, :cond_2

    .line 257
    invoke-virtual {p0, p2}, Lcom/rockstargames/gtasa/GTASA;->SetLocale(Ljava/lang/String;)V

    goto :goto_0

    .line 258
    :cond_2
    const-string v5, "IsAmazonBuild"

    invoke-virtual {p1, v5}, Ljava/lang/String;->equalsIgnoreCase(Ljava/lang/String;)Z

    move-result v5

    if-eqz v5, :cond_3

    .line 259
    iget-boolean v3, p0, Lcom/rockstargames/gtasa/GTASA;->IsAmazonBuild:Z

    goto :goto_0

    .line 260
    :cond_3
    const-string v5, "Download"

    invoke-virtual {p1, v5}, Ljava/lang/String;->equalsIgnoreCase(Ljava/lang/String;)Z

    move-result v5

    if-nez v5, :cond_0

    .line 268
    const-string v5, "IsDownloaded"

    invoke-virtual {p1, v5}, Ljava/lang/String;->equalsIgnoreCase(Ljava/lang/String;)Z

    move-result v5

    if-eqz v5, :cond_4

    .line 270
    :try_start_0
    const-string v5, ";"

    invoke-virtual {p2, v5}, Ljava/lang/String;->split(Ljava/lang/String;)[Ljava/lang/String;

    move-result-object v2

    .line 271
    .local v2, "part":[Ljava/lang/String;
    new-instance v0, Ljava/io/File;

    iget-object v5, p0, Lcom/rockstargames/gtasa/GTASA;->baseDirectory:Ljava/lang/String;

    const/4 v6, 0x0

    aget-object v6, v2, v6

    invoke-direct {v0, v5, v6}, Ljava/io/File;-><init>(Ljava/lang/String;Ljava/lang/String;)V

    .line 272
    .local v0, "f":Ljava/io/File;
    const/4 v5, 0x1

    aget-object v5, v2, v5

    invoke-static {v5}, Ljava/lang/Integer;->parseInt(Ljava/lang/String;)I

    move-result v1

    .line 273
    .local v1, "len":I
    invoke-virtual {v0}, Ljava/io/File;->length()J
    :try_end_0
    .catch Ljava/lang/Exception; {:try_start_0 .. :try_end_0} :catch_0

    move-result-wide v6

    int-to-long v8, v1

    cmp-long v5, v6, v8

    if-nez v5, :cond_0

    move v3, v4

    goto :goto_0

    .line 277
    .end local v0    # "f":Ljava/io/File;
    .end local v1    # "len":I
    .end local v2    # "part":[Ljava/lang/String;
    :cond_4
    const-string v4, "ADXEvent"

    invoke-virtual {p1, v4}, Ljava/lang/String;->equalsIgnoreCase(Ljava/lang/String;)Z

    move-result v4

    if-eqz v4, :cond_0

    .line 279
    sget-object v4, Lcom/rockstargames/gtasa/GTASA;->gtasaSelf:Lcom/rockstargames/gtasa/GTASA;

    const-string v5, ""

    const-string v6, ""

    invoke-static {v4, p2, v5, v6}, Lcom/AdX/tag/AdXConnect;->getAdXConnectEventInstance(Landroid/content/Context;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V

    goto :goto_0

    .line 274
    :catch_0
    move-exception v4

    goto :goto_0
.end method

.method public ServiceAppCommandValue(Ljava/lang/String;Ljava/lang/String;)I
    .locals 2
    .param p1, "cmd"    # Ljava/lang/String;
    .param p2, "args"    # Ljava/lang/String;

    .prologue
    const/4 v0, 0x0

    .line 291
    const-string v1, "GetDownloadBytes"

    invoke-virtual {p1, v1}, Ljava/lang/String;->equalsIgnoreCase(Ljava/lang/String;)Z

    move-result v1

    if-eqz v1, :cond_1

    .line 309
    :cond_0
    :goto_0
    return v0

    .line 297
    :cond_1
    const-string v1, "GetDownloadState"

    invoke-virtual {p1, v1}, Ljava/lang/String;->equalsIgnoreCase(Ljava/lang/String;)Z

    move-result v1

    if-eqz v1, :cond_2

    .line 302
    const/4 v0, 0x4

    goto :goto_0

    .line 303
    :cond_2
    const-string v1, "GetNetworkState"

    invoke-virtual {p1, v1}, Ljava/lang/String;->equalsIgnoreCase(Ljava/lang/String;)Z

    move-result v1

    if-eqz v1, :cond_0

    .line 304
    invoke-virtual {p0}, Lcom/rockstargames/gtasa/GTASA;->isNetworkAvailable()Z

    move-result v1

    if-eqz v1, :cond_0

    const/4 v0, 0x1

    goto :goto_0
.end method

.method public native main()V
.end method

.method public onActivityResult(IILandroid/content/Intent;)V
    .locals 0
    .param p1, "requestCode"    # I
    .param p2, "resultCode"    # I
    .param p3, "data"    # Landroid/content/Intent;

    .prologue
    .line 396
    invoke-super {p0, p1, p2, p3}, Lcom/wardrumstudios/utils/WarMedia;->onActivityResult(IILandroid/content/Intent;)V

    .line 398
    return-void
.end method

.method public onConfigurationChanged(Landroid/content/res/Configuration;)V
    .locals 6
    .param p1, "newConfig"    # Landroid/content/res/Configuration;

    .prologue
    const/4 v3, 0x0

    const/4 v2, 0x1

    .line 378
    invoke-super {p0, p1}, Lcom/wardrumstudios/utils/WarMedia;->onConfigurationChanged(Landroid/content/res/Configuration;)V

    .line 380
    iget-boolean v1, p0, Lcom/rockstargames/gtasa/GTASA;->isInSocialClub:Z

    if-nez v1, :cond_0

    .line 391
    :goto_0
    return-void

    .line 382
    :cond_0
    const-string v1, "StartupActivity"

    new-instance v4, Ljava/lang/StringBuilder;

    invoke-direct {v4}, Ljava/lang/StringBuilder;-><init>()V

    const-string v5, "*** MainActivity onConfigurationChanged() ("

    invoke-virtual {v4, v5}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v4

    sget v5, Lcom/rockstargames/gtasa/GTASA;->debugStaticCheck:I

    invoke-virtual {v4, v5}, Ljava/lang/StringBuilder;->append(I)Ljava/lang/StringBuilder;

    move-result-object v4

    const-string v5, ") ***)"

    invoke-virtual {v4, v5}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v4

    invoke-virtual {v4}, Ljava/lang/StringBuilder;->toString()Ljava/lang/String;

    move-result-object v4

    invoke-static {v1, v4}, Landroid/util/Log;->i(Ljava/lang/String;Ljava/lang/String;)I

    .line 383
    sget v1, Lcom/rockstargames/gtasa/GTASA;->debugStaticCheck:I

    add-int/lit8 v1, v1, 0x1

    sput v1, Lcom/rockstargames/gtasa/GTASA;->debugStaticCheck:I

    .line 385
    invoke-virtual {p0}, Lcom/rockstargames/gtasa/GTASA;->getWindowManager()Landroid/view/WindowManager;

    move-result-object v1

    invoke-interface {v1}, Landroid/view/WindowManager;->getDefaultDisplay()Landroid/view/Display;

    move-result-object v0

    .line 387
    .local v0, "d":Landroid/view/Display;
    invoke-virtual {p0}, Lcom/rockstargames/gtasa/GTASA;->getResources()Landroid/content/res/Resources;

    move-result-object v1

    invoke-virtual {v1}, Landroid/content/res/Resources;->getConfiguration()Landroid/content/res/Configuration;

    move-result-object v1

    iget v1, v1, Landroid/content/res/Configuration;->orientation:I

    if-ne v1, v2, :cond_1

    move v1, v2

    :goto_1
    invoke-virtual {v0}, Landroid/view/Display;->getWidth()I

    move-result v4

    invoke-virtual {v0}, Landroid/view/Display;->getHeight()I

    move-result v5

    if-ge v4, v5, :cond_2

    :goto_2
    if-ne v1, v2, :cond_3

    .line 388
    invoke-static {}, Lcom/rockstargames/hal/ActivityWrapper;->getInstance()Lcom/rockstargames/hal/ActivityWrapper;

    move-result-object v1

    invoke-virtual {v0}, Landroid/view/Display;->getWidth()I

    move-result v2

    invoke-virtual {v0}, Landroid/view/Display;->getHeight()I

    move-result v3

    invoke-virtual {v1, v2, v3}, Lcom/rockstargames/hal/ActivityWrapper;->setCurrentScreenSize(II)V

    goto :goto_0

    :cond_1
    move v1, v3

    .line 387
    goto :goto_1

    :cond_2
    move v2, v3

    goto :goto_2

    .line 390
    :cond_3
    invoke-static {}, Lcom/rockstargames/hal/ActivityWrapper;->getInstance()Lcom/rockstargames/hal/ActivityWrapper;

    move-result-object v1

    invoke-virtual {v0}, Landroid/view/Display;->getHeight()I

    move-result v2

    invoke-virtual {v0}, Landroid/view/Display;->getWidth()I

    move-result v3

    invoke-virtual {v1, v2, v3}, Lcom/rockstargames/hal/ActivityWrapper;->setCurrentScreenSize(II)V

    goto :goto_0
.end method

.method public onCreate(Landroid/os/Bundle;)V
    .locals 12
    .param p1, "savedInstanceState"    # Landroid/os/Bundle;

    .prologue
    const/16 v9, 0x8

    const/4 v6, 0x1

    const/4 v7, 0x0

    .line 61
    sput-object p0, Lcom/rockstargames/gtasa/GTASA;->gtasaSelf:Lcom/rockstargames/gtasa/GTASA;

    .line 66
    const/4 v2, 0x0

    .line 67
    .local v2, "isupdate":Z
    const/4 v3, 0x0

    .line 68
    .local v3, "loglevel":I
    sget-object v5, Lcom/rockstargames/gtasa/GTASA;->gtasaSelf:Lcom/rockstargames/gtasa/GTASA;

    invoke-static {v5, v2, v7}, Lcom/AdX/tag/AdXConnect;->getAdXConnectInstance(Landroid/content/Context;ZI)Lcom/AdX/tag/AdXConnect;

    .line 70
    const v5, 0x1e1bc

    iput v5, p0, Lcom/rockstargames/gtasa/GTASA;->HELLO_ID:I

    .line 71
    new-instance v5, Landroid/content/Intent;

    const-class v8, Lcom/rockstargames/gtasa/GTASA;

    invoke-direct {v5, p0, v8}, Landroid/content/Intent;-><init>(Landroid/content/Context;Ljava/lang/Class;)V

    iput-object v5, p0, Lcom/rockstargames/gtasa/GTASA;->appIntent:Landroid/content/Intent;

    .line 72
    const-string v5, "GTA3 San Andreas"

    iput-object v5, p0, Lcom/rockstargames/gtasa/GTASA;->appTickerText:Ljava/lang/CharSequence;

    .line 73
    const-string v5, "San Andreas"

    iput-object v5, p0, Lcom/rockstargames/gtasa/GTASA;->appContentTitle:Ljava/lang/CharSequence;

    .line 74
    const-string v5, "Running - Return to Game?"

    iput-object v5, p0, Lcom/rockstargames/gtasa/GTASA;->appContentText:Ljava/lang/CharSequence;

    .line 75
    const-string v5, "com.rockstargames.gtasa.R"

    sput-object v5, Lcom/google/android/vending/expansion/downloader/Helpers;->resourceClassString:Ljava/lang/String;

    .line 77
    const-string v5, "main.8.com.rockstargames.gtasa.obb"

    iput-object v5, p0, Lcom/rockstargames/gtasa/GTASA;->expansionFileName:Ljava/lang/String;

    .line 78
    const-string v5, "patch.8.com.rockstargames.gtasa.obb"

    iput-object v5, p0, Lcom/rockstargames/gtasa/GTASA;->patchFileName:Ljava/lang/String;

    .line 80
    const-string v5, "com.rockstargames.gtasa"

    invoke-virtual {p0, v5}, Lcom/rockstargames/gtasa/GTASA;->GetPackageName(Ljava/lang/String;)Ljava/lang/String;

    move-result-object v5

    iput-object v5, p0, Lcom/rockstargames/gtasa/GTASA;->apkFileName:Ljava/lang/String;

    .line 82
    invoke-virtual {p0}, Lcom/rockstargames/gtasa/GTASA;->GetGameBaseDirectory()Ljava/lang/String;

    move-result-object v5

    iput-object v5, p0, Lcom/rockstargames/gtasa/GTASA;->baseDirectory:Ljava/lang/String;

    .line 83
    iput-boolean v6, p0, Lcom/rockstargames/gtasa/GTASA;->AllowLongPressForExit:Z

    .line 86
    new-instance v5, Ljava/lang/StringBuilder;

    invoke-direct {v5}, Ljava/lang/StringBuilder;-><init>()V

    invoke-static {}, Landroid/os/Environment;->getExternalStorageDirectory()Ljava/io/File;

    move-result-object v8

    invoke-virtual {v5, v8}, Ljava/lang/StringBuilder;->append(Ljava/lang/Object;)Ljava/lang/StringBuilder;

    move-result-object v5

    const-string v8, "/GTASA/"

    invoke-virtual {v5, v8}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v5

    invoke-virtual {v5}, Ljava/lang/StringBuilder;->toString()Ljava/lang/String;

    move-result-object v1

    .line 87
    .local v1, "externalDirectory":Ljava/lang/String;
    new-instance v5, Ljava/lang/StringBuilder;

    invoke-direct {v5}, Ljava/lang/StringBuilder;-><init>()V

    invoke-virtual {v5, v1}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v5

    iget-object v8, p0, Lcom/rockstargames/gtasa/GTASA;->expansionFileName:Ljava/lang/String;

    invoke-virtual {v5, v8}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v5

    invoke-virtual {v5}, Ljava/lang/StringBuilder;->toString()Ljava/lang/String;

    move-result-object v4

    .line 88
    .local v4, "testExpansionFileName":Ljava/lang/String;
    new-instance v0, Ljava/io/File;

    invoke-direct {v0, v4}, Ljava/io/File;-><init>(Ljava/lang/String;)V

    .line 90
    .local v0, "buildobb":Ljava/io/File;
    iget-boolean v5, p0, Lcom/rockstargames/gtasa/GTASA;->IsAmazonBuild:Z

    if-nez v5, :cond_0

    invoke-virtual {v0}, Ljava/io/File;->exists()Z

    move-result v5

    if-eqz v5, :cond_0

    .line 91
    iput-boolean v7, p0, Lcom/rockstargames/gtasa/GTASA;->UseExpansionPack:Z

    .line 92
    iget-object v5, p0, Lcom/rockstargames/gtasa/GTASA;->expansionFileName:Ljava/lang/String;

    iput-object v5, p0, Lcom/rockstargames/gtasa/GTASA;->expansionFileName:Ljava/lang/String;

    .line 93
    iput-object v1, p0, Lcom/rockstargames/gtasa/GTASA;->baseDirectory:Ljava/lang/String;

    .line 95
    :cond_0
    iget-boolean v5, p0, Lcom/rockstargames/gtasa/GTASA;->IsAmazonBuild:Z

    if-eqz v5, :cond_1

    iput-boolean v7, p0, Lcom/rockstargames/gtasa/GTASA;->UseExpansionPack:Z

    .line 97
    :cond_1
    const-string v5, "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAlDfyMGss+/15UlnoCk+/NWLga5rXTgJyt893tWIPAkSNawLOHC8pFzuKZ26ZxGKUB88+6kfXVHBryUb3pDWItMj9qbDm+6Guu3mDx+r9TmSDEN8olB2egFSeosjWT4wvFu/couZumEGKgAqcIl82LIfblyGmkh9zzgyf3MSQnQdUbtIpC4uoYs51f9jEalFrtAWb7gGrPFKbMydRZgADO+Uon1w9Q+Zv5Jmtgg0YpA1hxQRf9eEidlz1Rry1U+/HtfZH7oHKIiDFm+7EmRwX4qacxVr/xdM5AUvI5GMgij37i+eAxQAq267mRDOy3UZc42odygjvGqL8ln9+cUfudwIDAQAB"

    sput-object v5, Lcom/wardrumstudios/utils/WarDownloaderService;->BASE64_PUBLIC_KEY:Ljava/lang/String;

    .line 99
    const/16 v5, 0x14

    new-array v5, v5, [B

    fill-array-data v5, :array_0

    sput-object v5, Lcom/wardrumstudios/utils/WarDownloaderService;->SALT:[B

    .line 100
    iget-boolean v5, p0, Lcom/rockstargames/gtasa/GTASA;->UseExpansionPack:Z

    if-eqz v5, :cond_2

    .line 101
    const/4 v5, 0x2

    new-array v5, v5, [Lcom/wardrumstudios/utils/WarMedia$XAPKFile;

    iput-object v5, p0, Lcom/rockstargames/gtasa/GTASA;->xAPKS:[Lcom/wardrumstudios/utils/WarMedia$XAPKFile;

    .line 103
    iget-object v5, p0, Lcom/rockstargames/gtasa/GTASA;->xAPKS:[Lcom/wardrumstudios/utils/WarMedia$XAPKFile;

    new-instance v8, Lcom/wardrumstudios/utils/WarMedia$XAPKFile;

    const-wide/32 v10, 0x75469c7c

    invoke-direct {v8, v6, v9, v10, v11}, Lcom/wardrumstudios/utils/WarMedia$XAPKFile;-><init>(ZIJ)V

    aput-object v8, v5, v7

    .line 108
    iget-object v5, p0, Lcom/rockstargames/gtasa/GTASA;->xAPKS:[Lcom/wardrumstudios/utils/WarMedia$XAPKFile;

    new-instance v8, Lcom/wardrumstudios/utils/WarMedia$XAPKFile;

    const-wide/32 v10, 0x254584f6

    invoke-direct {v8, v7, v9, v10, v11}, Lcom/wardrumstudios/utils/WarMedia$XAPKFile;-><init>(ZIJ)V

    aput-object v8, v5, v6

    .line 117
    :cond_2
    iput-boolean v7, p0, Lcom/rockstargames/gtasa/GTASA;->AddMovieExtension:Z

    .line 119
    iput-boolean v6, p0, Lcom/rockstargames/gtasa/GTASA;->wantsMultitouch:Z

    .line 120
    iput-boolean v6, p0, Lcom/rockstargames/gtasa/GTASA;->wantsAccelerometer:Z

    .line 122
    invoke-virtual {p0}, Lcom/rockstargames/gtasa/GTASA;->RestoreCurrentLanguage()V

    .line 124
    sget-boolean v5, Lcom/rockstargames/gtasa/GTASA;->UseAndroidHal:Z

    if-eqz v5, :cond_5

    invoke-virtual {p0}, Lcom/rockstargames/gtasa/GTASA;->IsTV()Z

    move-result v5

    if-nez v5, :cond_5

    move v5, v6

    :goto_0
    sput-boolean v5, Lcom/rockstargames/gtasa/GTASA;->UseAndroidHal:Z

    .line 125
    sget-boolean v5, Lcom/rockstargames/gtasa/GTASA;->UseAndroidHal:Z

    if-eqz v5, :cond_3

    .line 126
    iput-boolean v6, p0, Lcom/rockstargames/gtasa/GTASA;->delaySetContentView:Z

    .line 129
    :cond_3
    invoke-super {p0, p1}, Lcom/wardrumstudios/utils/WarMedia;->onCreate(Landroid/os/Bundle;)V

    .line 131
    invoke-virtual {p0, v7}, Lcom/rockstargames/gtasa/GTASA;->SetReportPS3As360(Z)V

    .line 133
    sget-boolean v5, Lcom/rockstargames/gtasa/GTASA;->UseAndroidHal:Z

    if-nez v5, :cond_4

    .line 134
    iput-boolean v7, p0, Lcom/rockstargames/gtasa/GTASA;->isInSocialClub:Z

    .line 144
    :cond_4
    return-void

    :cond_5
    move v5, v7

    .line 124
    goto :goto_0

    .line 99
    nop

    :array_0
    .array-data 1
        0x1t
        0x2at
        -0xct
        -0x1t
        0x36t
        0x62t
        -0x64t
        -0xct
        0x2bt
        0x2t
        -0x8t
        -0x4t
        0x9t
        0x5t
        -0x6at
        -0x6bt
        -0x21t
        0x2dt
        -0x1t
        0x54t
    .end array-data
.end method

.method public onDestroy()V
    .locals 3

    .prologue
    .line 369
    invoke-super {p0}, Lcom/wardrumstudios/utils/WarMedia;->onDestroy()V

    .line 370
    iget-boolean v0, p0, Lcom/rockstargames/gtasa/GTASA;->isInSocialClub:Z

    if-nez v0, :cond_0

    .line 373
    :goto_0
    return-void

    .line 371
    :cond_0
    const-string v0, "StartupActivity"

    new-instance v1, Ljava/lang/StringBuilder;

    invoke-direct {v1}, Ljava/lang/StringBuilder;-><init>()V

    const-string v2, "*** MainActivity onDestroy() ("

    invoke-virtual {v1, v2}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v1

    sget v2, Lcom/rockstargames/gtasa/GTASA;->debugStaticCheck:I

    invoke-virtual {v1, v2}, Ljava/lang/StringBuilder;->append(I)Ljava/lang/StringBuilder;

    move-result-object v1

    const-string v2, ") ***)"

    invoke-virtual {v1, v2}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v1

    invoke-virtual {v1}, Ljava/lang/StringBuilder;->toString()Ljava/lang/String;

    move-result-object v1

    invoke-static {v0, v1}, Landroid/util/Log;->i(Ljava/lang/String;Ljava/lang/String;)I

    .line 372
    sget v0, Lcom/rockstargames/gtasa/GTASA;->debugStaticCheck:I

    add-int/lit8 v0, v0, 0x1

    sput v0, Lcom/rockstargames/gtasa/GTASA;->debugStaticCheck:I

    goto :goto_0
.end method

.method public onKeyDown(ILandroid/view/KeyEvent;)Z
    .locals 2
    .param p1, "keyCode"    # I
    .param p2, "event"    # Landroid/view/KeyEvent;

    .prologue
    const/4 v0, 0x1

    .line 188
    const/4 v1, 0x4

    if-ne p1, v1, :cond_0

    .line 190
    iget-boolean v1, p0, Lcom/rockstargames/gtasa/GTASA;->isInSocialClub:Z

    if-eqz v1, :cond_0

    .line 191
    invoke-static {}, Lcom/rockstargames/hal/ActivityWrapper;->getInstance()Lcom/rockstargames/hal/ActivityWrapper;

    move-result-object v1

    invoke-virtual {v1}, Lcom/rockstargames/hal/ActivityWrapper;->onBackPressed()V

    .line 206
    :goto_0
    return v0

    .line 196
    :cond_0
    iget-boolean v1, p0, Lcom/rockstargames/gtasa/GTASA;->isInSocialClub:Z

    if-eqz v1, :cond_2

    .line 197
    const/16 v1, 0x60

    if-eq p1, v1, :cond_1

    const/16 v1, 0x61

    if-eq p1, v1, :cond_1

    const/16 v1, 0x63

    if-eq p1, v1, :cond_1

    const/16 v1, 0x64

    if-eq p1, v1, :cond_1

    const/16 v1, 0x6d

    if-eq p1, v1, :cond_1

    const/16 v1, 0x6c

    if-eq p1, v1, :cond_1

    const/16 v1, 0x67

    if-eq p1, v1, :cond_1

    const/16 v1, 0x66

    if-ne p1, v1, :cond_2

    .line 202
    :cond_1
    invoke-static {}, Lcom/rockstargames/hal/ActivityWrapper;->getInstance()Lcom/rockstargames/hal/ActivityWrapper;

    move-result-object v1

    invoke-virtual {v1}, Lcom/rockstargames/hal/ActivityWrapper;->onExitSC()V

    goto :goto_0

    .line 206
    :cond_2
    invoke-super {p0, p1, p2}, Lcom/wardrumstudios/utils/WarMedia;->onKeyDown(ILandroid/view/KeyEvent;)Z

    move-result v0

    goto :goto_0
.end method

.method public onPause()V
    .locals 3

    .prologue
    .line 348
    invoke-super {p0}, Lcom/wardrumstudios/utils/WarMedia;->onPause()V

    .line 349
    iget-boolean v0, p0, Lcom/rockstargames/gtasa/GTASA;->isInSocialClub:Z

    if-nez v0, :cond_0

    .line 354
    :goto_0
    return-void

    .line 350
    :cond_0
    invoke-static {}, Lcom/rockstargames/hal/ActivityWrapper;->onPauseCallback()V

    .line 352
    const-string v0, "StartupActivity"

    new-instance v1, Ljava/lang/StringBuilder;

    invoke-direct {v1}, Ljava/lang/StringBuilder;-><init>()V

    const-string v2, "*** MainActivity onPause() ("

    invoke-virtual {v1, v2}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v1

    sget v2, Lcom/rockstargames/gtasa/GTASA;->debugStaticCheck:I

    invoke-virtual {v1, v2}, Ljava/lang/StringBuilder;->append(I)Ljava/lang/StringBuilder;

    move-result-object v1

    const-string v2, ") ***)"

    invoke-virtual {v1, v2}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v1

    invoke-virtual {v1}, Ljava/lang/StringBuilder;->toString()Ljava/lang/String;

    move-result-object v1

    invoke-static {v0, v1}, Landroid/util/Log;->i(Ljava/lang/String;Ljava/lang/String;)I

    .line 353
    sget v0, Lcom/rockstargames/gtasa/GTASA;->debugStaticCheck:I

    add-int/lit8 v0, v0, 0x1

    sput v0, Lcom/rockstargames/gtasa/GTASA;->debugStaticCheck:I

    goto :goto_0
.end method

.method public onRestart()V
    .locals 3

    .prologue
    .line 327
    invoke-super {p0}, Lcom/wardrumstudios/utils/WarMedia;->onRestart()V

    .line 328
    iget-boolean v0, p0, Lcom/rockstargames/gtasa/GTASA;->isInSocialClub:Z

    if-nez v0, :cond_0

    .line 331
    :goto_0
    return-void

    .line 329
    :cond_0
    const-string v0, "StartupActivity"

    new-instance v1, Ljava/lang/StringBuilder;

    invoke-direct {v1}, Ljava/lang/StringBuilder;-><init>()V

    const-string v2, "*** MainActivity onRestart() ("

    invoke-virtual {v1, v2}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v1

    sget v2, Lcom/rockstargames/gtasa/GTASA;->debugStaticCheck:I

    invoke-virtual {v1, v2}, Ljava/lang/StringBuilder;->append(I)Ljava/lang/StringBuilder;

    move-result-object v1

    const-string v2, ") ***)"

    invoke-virtual {v1, v2}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v1

    invoke-virtual {v1}, Ljava/lang/StringBuilder;->toString()Ljava/lang/String;

    move-result-object v1

    invoke-static {v0, v1}, Landroid/util/Log;->i(Ljava/lang/String;Ljava/lang/String;)I

    .line 330
    sget v0, Lcom/rockstargames/gtasa/GTASA;->debugStaticCheck:I

    add-int/lit8 v0, v0, 0x1

    sput v0, Lcom/rockstargames/gtasa/GTASA;->debugStaticCheck:I

    goto :goto_0
.end method

.method public onResume()V
    .locals 3

    .prologue
    .line 336
    invoke-super {p0}, Lcom/wardrumstudios/utils/WarMedia;->onResume()V

    .line 337
    iget-boolean v0, p0, Lcom/rockstargames/gtasa/GTASA;->waitForPermissions:Z

    if-eqz v0, :cond_1

    .line 343
    :cond_0
    :goto_0
    return-void

    .line 338
    :cond_1
    iget-boolean v0, p0, Lcom/rockstargames/gtasa/GTASA;->isInSocialClub:Z

    if-eqz v0, :cond_0

    .line 339
    invoke-static {}, Lcom/rockstargames/hal/ActivityWrapper;->onResumeCallback()V

    .line 341
    const-string v0, "StartupActivity"

    new-instance v1, Ljava/lang/StringBuilder;

    invoke-direct {v1}, Ljava/lang/StringBuilder;-><init>()V

    const-string v2, "*** MainActivity onResume() ("

    invoke-virtual {v1, v2}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v1

    sget v2, Lcom/rockstargames/gtasa/GTASA;->debugStaticCheck:I

    invoke-virtual {v1, v2}, Ljava/lang/StringBuilder;->append(I)Ljava/lang/StringBuilder;

    move-result-object v1

    const-string v2, ") ***)"

    invoke-virtual {v1, v2}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v1

    invoke-virtual {v1}, Ljava/lang/StringBuilder;->toString()Ljava/lang/String;

    move-result-object v1

    invoke-static {v0, v1}, Landroid/util/Log;->i(Ljava/lang/String;Ljava/lang/String;)I

    .line 342
    sget v0, Lcom/rockstargames/gtasa/GTASA;->debugStaticCheck:I

    add-int/lit8 v0, v0, 0x1

    sput v0, Lcom/rockstargames/gtasa/GTASA;->debugStaticCheck:I

    goto :goto_0
.end method

.method public onStart()V
    .locals 3

    .prologue
    .line 316
    invoke-super {p0}, Lcom/wardrumstudios/utils/WarMedia;->onStart()V

    .line 317
    iget-boolean v0, p0, Lcom/rockstargames/gtasa/GTASA;->isInSocialClub:Z

    if-nez v0, :cond_0

    .line 322
    :goto_0
    return-void

    .line 318
    :cond_0
    invoke-static {}, Lcom/rockstargames/hal/ActivityWrapper;->onStartCallback()V

    .line 320
    const-string v0, "StartupActivity"

    new-instance v1, Ljava/lang/StringBuilder;

    invoke-direct {v1}, Ljava/lang/StringBuilder;-><init>()V

    const-string v2, "*** MainActivity onStart() ("

    invoke-virtual {v1, v2}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v1

    sget v2, Lcom/rockstargames/gtasa/GTASA;->debugStaticCheck:I

    invoke-virtual {v1, v2}, Ljava/lang/StringBuilder;->append(I)Ljava/lang/StringBuilder;

    move-result-object v1

    const-string v2, ") ***)"

    invoke-virtual {v1, v2}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v1

    invoke-virtual {v1}, Ljava/lang/StringBuilder;->toString()Ljava/lang/String;

    move-result-object v1

    invoke-static {v0, v1}, Landroid/util/Log;->i(Ljava/lang/String;Ljava/lang/String;)I

    .line 321
    sget v0, Lcom/rockstargames/gtasa/GTASA;->debugStaticCheck:I

    add-int/lit8 v0, v0, 0x1

    sput v0, Lcom/rockstargames/gtasa/GTASA;->debugStaticCheck:I

    goto :goto_0
.end method

.method public onStop()V
    .locals 3

    .prologue
    .line 358
    invoke-super {p0}, Lcom/wardrumstudios/utils/WarMedia;->onStop()V

    .line 359
    iget-boolean v0, p0, Lcom/rockstargames/gtasa/GTASA;->isInSocialClub:Z

    if-nez v0, :cond_0

    .line 364
    :goto_0
    return-void

    .line 362
    :cond_0
    const-string v0, "StartupActivity"

    new-instance v1, Ljava/lang/StringBuilder;

    invoke-direct {v1}, Ljava/lang/StringBuilder;-><init>()V

    const-string v2, "*** MainActivity onStop() ("

    invoke-virtual {v1, v2}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v1

    sget v2, Lcom/rockstargames/gtasa/GTASA;->debugStaticCheck:I

    invoke-virtual {v1, v2}, Ljava/lang/StringBuilder;->append(I)Ljava/lang/StringBuilder;

    move-result-object v1

    const-string v2, ") ***)"

    invoke-virtual {v1, v2}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v1

    invoke-virtual {v1}, Ljava/lang/StringBuilder;->toString()Ljava/lang/String;

    move-result-object v1

    invoke-static {v0, v1}, Landroid/util/Log;->i(Ljava/lang/String;Ljava/lang/String;)I

    .line 363
    sget v0, Lcom/rockstargames/gtasa/GTASA;->debugStaticCheck:I

    add-int/lit8 v0, v0, 0x1

    sput v0, Lcom/rockstargames/gtasa/GTASA;->debugStaticCheck:I

    goto :goto_0
.end method

.method public native setCurrentScreenSize(II)V
.end method
