//  $Id$
// 
//  SuperTux
//  Copyright (C) 2000 Bill Kendrick <bill@newbreedsoftware.com>
//  Copyright (C) 2004 Tobias Glaesser <tobi.web@gmx.de>
//  Copyright (C) 2004 Matthias Braun <matze@braunis.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
// 
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
//  02111-1307, USA.

#ifndef SUPERTUX_BADGUY_H
#define SUPERTUX_BADGUY_H

#include "SDL.h"
#include "defines.h"
#include "bitmask.h"
#include "type.h"
#include "timer.h"
#include "texture.h"
#include "physic.h"
#include "collision.h"
#include "sprite.h"
#include "moving_object.h"
#include "drawable.h"
#include "serializable.h"

/* Bad guy kinds: */
enum BadGuyKind {
  BAD_MRICEBLOCK,
  BAD_JUMPY,
  BAD_MRBOMB,
  BAD_BOMB,
  BAD_STALACTITE,
  BAD_FLAME,
  BAD_FISH,
  BAD_BOUNCINGSNOWBALL,
  BAD_FLYINGSNOWBALL,
  BAD_SPIKY,
  BAD_SNOWBALL,
  NUM_BadGuyKinds
};

BadGuyKind  badguykind_from_string(const std::string& str);
std::string badguykind_to_string(BadGuyKind kind);
void load_badguy_gfx();
void free_badguy_gfx();

class Player;

/* Badguy type: */
class BadGuy : public MovingObject, public Drawable, public Serializable
{
public:
  /* Enemy modes: */
  enum BadGuyMode {
    NORMAL=0,
    FLAT,
    KICK,
    HELD,

    JUMPY_JUMP,

    BOMB_TICKING,
    BOMB_EXPLODE,

    STALACTITE_SHAKING,
    STALACTITE_FALL,

    FISH_WAIT,

    FLY_UP,
    FLY_DOWN
  };
public:
  DyingType  dying;
  BadGuyKind kind;
  BadGuyMode mode;

  /** If true the enemy will stay on its current platform, ie. if he
      reaches the edge he will turn around and walk into the other
      direction, if false the enemy will jump or walk of the edge */
  bool stay_on_platform;

  Direction dir;

  Timer frozen_timer;  // gets frozen when a ice shot hits it

private:
  bool removable;
  bool seen;
  int squishcount; /// number of times this enemy was squiched
  Timer timer;
  Physic physic;
  float angle;

  Sprite*   sprite_left;
  Sprite*   sprite_right;

  int animation_offset;

public:
  BadGuy(DisplayManager& display_manager, BadGuyKind kind, float x, float y);
  BadGuy(DisplayManager& display_manager, BadGuyKind kind, LispReader& reader);
  virtual ~BadGuy();

  virtual void write(LispWriter& writer);

  virtual void action(float frame_ratio);
  virtual void draw(Camera& viewport, int layer);
  virtual void collision(const MovingObject& other, int type);

  void collision(void* p_c_object, int c_object,
                 CollisionType type = COLLISION_NORMAL);

  /** this functions tries to kill the badguy and lets him fall off the
   * screen. Some badguys like the flame might ignore this.
   */
  void kill_me(int score);

private:
  void init();
  
  void action_mriceblock(double frame_ratio);
  void action_jumpy(double frame_ratio); 
  void action_bomb(double frame_ratio);
  void action_mrbomb(double frame_ratio);
  void action_stalactite(double frame_ratio);
  void action_flame(double frame_ratio);
  void action_fish(double frame_ratio);
  void action_bouncingsnowball(double frame_ratio);
  void action_flyingsnowball(double frame_ratio);
  void action_spiky(double frame_ratio);
  void action_snowball(double frame_ratio);

  /** handles falling down. disables gravity calculation when we're back on
   * ground */
  void fall();

  /** let the player jump a bit (used when you hit a badguy) */
  void make_player_jump(Player* player);

  void explode();

  /** check if we're running left or right in a wall and eventually change
   * direction
   */
  void check_horizontal_bump(bool checkcliff = false);
  /** called when we're bumped from below with a block */
  void bump();
  /** called when a player jumped on the badguy from above */
  void squish(Player* player);
  /** squish ourself, give player score and set dying to DYING_SQICHED */
  void squish_me(Player* player);
  /** set image of the badguy */
  void set_sprite(Sprite* left, Sprite* right);
};

#endif /*SUPERTUX_BADGUY_H*/

/* Local Variables: */
/* mode:c++ */
/* End: */

