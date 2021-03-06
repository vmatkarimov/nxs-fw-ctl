#ifndef _INCLUDE_NXS_FW_CTL_U_COLLECTIONS_C
#define _INCLUDE_NXS_FW_CTL_U_COLLECTIONS_C
#endif

// clang-format off

#include <nxs-core/nxs-core.h>

/* Project core include */
#include <nxs-fw-ctl-core.h>
#include <nxs-fw-ctl-meta.h>
#include <nxs-fw-ctl-collections.h>
#include <nxs-fw-ctl-dal.h>

#include <units/collections/collections-typedefs.h>
#include <units/collections/collections.h>
#include <units/collections/collections-subunits.h>

/* Definitions */

#define NXS_FW_CTL_U_COLLS_TPL_C_FILENAME			"%%NXS_TPL_C_FILENAME%%"
#define NXS_FW_CTL_U_COLLS_TPL_C_RPATH				"%%NXS_TPL_C_RPATH%%"
#define NXS_FW_CTL_U_COLLS_TPL_C_OBJ				"%%NXS_TPL_C_OBJ%%"
#define NXS_FW_CTL_U_COLLS_TPL_C_UPCASE_NAME			"%%NXS_TPL_C_UPCASE_NAME%%"
#define NXS_FW_CTL_U_COLLS_TPL_C_INLINE_NAME			"%%NXS_TPL_C_INLINE_NAME%%"
#define NXS_FW_CTL_U_COLLS_TPL_P_NAME				"%%NXS_TPL_P_NAME%%"
#define NXS_FW_CTL_U_COLLS_TPL_P_INLINE_NAME			"%%NXS_TPL_P_INLINE_NAME%%"

#define NXS_FW_CTL_U_COLLS_TPL_C_HEADERS_B			"/*%%_NXS_TPL_HEADERS_BEGIN_%%*/"
#define NXS_FW_CTL_U_COLLS_TPL_C_HEADERS_E			"/*%%_NXS_TPL_HEADERS_END_%%*/"

/* Project globals */
extern		nxs_process_t					process;
extern		nxs_fw_ctl_cfg_t				nxs_fw_ctl_cfg;

/* Module typedefs */

typedef struct							nxs_fw_ctl_u_collections_path_s				nxs_fw_ctl_u_collections_path_t;

/* Module declarations */

struct nxs_fw_ctl_u_collections_path_s
{
	nxs_string_t						src;
	nxs_string_t						dst;
	mode_t							mode;
};

struct nxs_fw_ctl_u_collections_s
{
	nxs_string_t						name;
	nxs_string_t						inline_name;
	nxs_string_t						upcase_name;

	nxs_string_t						obj_name;
	nxs_string_t						ppath;
	nxs_string_t						path;
	nxs_string_t						rpath;

	nxs_string_t						proj_name;
	nxs_string_t						proj_inline_name;
	nxs_string_t						proj_upcase_name;

	nxs_string_t						proj_root;
};

/* Module internal (static) functions prototypes */

// clang-format on

static nxs_fw_ctl_err_t nxs_fw_ctl_u_collections_make_fs_struct(nxs_fw_ctl_u_collections_t *u_ctx);
static nxs_fw_ctl_err_t nxs_fw_ctl_u_collections_make_makefiles(nxs_fw_ctl_u_collections_t *u_ctx, nxs_string_t *fw_version);
static nxs_fw_ctl_err_t nxs_fw_ctl_u_collections_make_genfiles(nxs_fw_ctl_u_collections_t *u_ctx, nxs_string_t *fw_version);
static nxs_fw_ctl_err_t nxs_fw_ctl_u_collections_make_sub_headers(nxs_fw_ctl_u_collections_t *u_ctx);

// clang-format off

/* Module initializations */

static nxs_string_t _s_tpl_c_filename				= nxs_string(NXS_FW_CTL_U_COLLS_TPL_C_FILENAME);
static nxs_string_t _s_tpl_c_rpath				= nxs_string(NXS_FW_CTL_U_COLLS_TPL_C_RPATH);
static nxs_string_t _s_tpl_c_obj				= nxs_string(NXS_FW_CTL_U_COLLS_TPL_C_OBJ);
static nxs_string_t _s_tpl_c_upcase_name			= nxs_string(NXS_FW_CTL_U_COLLS_TPL_C_UPCASE_NAME);
static nxs_string_t _s_tpl_c_inline_name			= nxs_string(NXS_FW_CTL_U_COLLS_TPL_C_INLINE_NAME);
static nxs_string_t _s_tpl_c_p_name				= nxs_string(NXS_FW_CTL_U_COLLS_TPL_P_NAME);
static nxs_string_t _s_tpl_c_p_inline_name			= nxs_string(NXS_FW_CTL_U_COLLS_TPL_P_INLINE_NAME);

static nxs_string_t _s_tpl_c_headers_b				= nxs_string(NXS_FW_CTL_U_COLLS_TPL_C_HEADERS_B);
static nxs_string_t _s_tpl_c_headers_e				= nxs_string(NXS_FW_CTL_U_COLLS_TPL_C_HEADERS_E);

static nxs_string_t _s_question_collection_del			= nxs_string("\tyou realy want to delete this collection? [y/n]: ");

static nxs_fw_ctl_u_collections_path_t makefiles[] =
{
	{nxs_string("Makefile_tpl"),		nxs_string("Makefile"),						NXS_FW_CTL_FILE_MODE_DEF},

	{{NULL, 0, 0}, {NULL, 0, 0}, 0}
};

static nxs_fw_ctl_u_collections_path_t genfiles[] =
{
	{nxs_string("collection.c_tpl"),	nxs_string("" NXS_FW_CTL_U_COLLS_TPL_C_FILENAME ".c"),		NXS_FW_CTL_FILE_MODE_DEF},
	{nxs_string("collection.h_tpl"),	nxs_string("" NXS_FW_CTL_U_COLLS_TPL_C_FILENAME ".h"),		NXS_FW_CTL_FILE_MODE_DEF},

	{{NULL, 0, 0}, {NULL, 0, 0}, 0},
};

/* Module global functions */

// clang-format on

nxs_fw_ctl_u_collections_t *nxs_fw_ctl_u_collections_init(void)
{
	nxs_fw_ctl_u_collections_t *u_ctx;

	u_ctx = (nxs_fw_ctl_u_collections_t *)nxs_malloc(NULL, sizeof(nxs_fw_ctl_u_collections_t));

	nxs_string_init(&u_ctx->name);
	nxs_string_init(&u_ctx->inline_name);
	nxs_string_init(&u_ctx->upcase_name);

	nxs_string_init(&u_ctx->obj_name);
	nxs_string_init(&u_ctx->ppath);
	nxs_string_init(&u_ctx->path);
	nxs_string_init(&u_ctx->rpath);

	nxs_string_init(&u_ctx->proj_name);
	nxs_string_init(&u_ctx->proj_inline_name);
	nxs_string_init(&u_ctx->proj_upcase_name);

	nxs_string_init(&u_ctx->proj_root);

	return u_ctx;
}

nxs_fw_ctl_u_collections_t *nxs_fw_ctl_u_collections_free(nxs_fw_ctl_u_collections_t *u_ctx)
{

	if(u_ctx == NULL) {

		return NULL;
	}

	nxs_string_free(&u_ctx->name);
	nxs_string_free(&u_ctx->inline_name);
	nxs_string_free(&u_ctx->upcase_name);

	nxs_string_free(&u_ctx->obj_name);
	nxs_string_free(&u_ctx->ppath);
	nxs_string_free(&u_ctx->path);
	nxs_string_free(&u_ctx->rpath);

	nxs_string_free(&u_ctx->proj_name);
	nxs_string_free(&u_ctx->proj_inline_name);
	nxs_string_free(&u_ctx->proj_upcase_name);

	nxs_string_free(&u_ctx->proj_root);

	return (nxs_fw_ctl_u_collections_t *)nxs_free(u_ctx);
}

void nxs_fw_ctl_u_collections_setup(nxs_fw_ctl_u_collections_t *u_ctx,
                                    nxs_string_t *              proj_name,
                                    nxs_string_t *              proj_root,
                                    nxs_string_t *              c_name)
{
	nxs_string_t c_full_name;

	nxs_string_init(&c_full_name);

	/*
	 * Подготовка переменных
	 */

	/* name */

	nxs_string_cpy(&u_ctx->name, 0, c_name, 0);

	/* path, rpath, ppath */

	nxs_string_printf(&u_ctx->path, "%r/src/collections/%r/", proj_root, c_name);
	nxs_string_printf(&u_ctx->rpath, "%r/", c_name);
	nxs_string_printf(&u_ctx->ppath, "%r/src/collections/", proj_root);

	/* unit_obj, unit_upcase_name, unit_inline_name */

	nxs_string_printf(&c_full_name, "%r-c-%r", proj_name, c_name);

	nxs_fw_ctl_c_set_names(&c_full_name, &u_ctx->obj_name, &u_ctx->upcase_name, &u_ctx->inline_name);

	/* proj_root */

	nxs_string_cpy(&u_ctx->proj_root, 0, proj_root, 0);

	/* proj_name */

	nxs_fw_ctl_c_set_names(proj_name, &u_ctx->proj_name, &u_ctx->proj_upcase_name, &u_ctx->proj_inline_name);

	nxs_string_free(&c_full_name);
}

nxs_fw_ctl_err_t nxs_fw_ctl_u_collections_add(nxs_fw_ctl_u_collections_t *u_ctx, nxs_string_t *fw_version)
{
	nxs_fw_ctl_err_t rc;

	rc = NXS_FW_CTL_E_OK;

	if(nxs_fw_ctl_u_collections_make_fs_struct(u_ctx) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_collections_make_makefiles(u_ctx, fw_version) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_collections_make_genfiles(u_ctx, fw_version) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_collections_make_sub_headers(u_ctx) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

error:

	return rc;
}

nxs_fw_ctl_err_t nxs_fw_ctl_u_collections_del(nxs_fw_ctl_u_collections_t *u_ctx)
{
	nxs_fw_ctl_err_t rc;
	nxs_string_t     objs_path;

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&objs_path);

	nxs_string_printf(&objs_path, "%r/objs/", &u_ctx->proj_root);

	nxs_log_write_console(&process, "\tthis collection will be deleted from project '%r':", &u_ctx->proj_name);
	nxs_log_write_console(&process, "\t* %r", &u_ctx->name);

	if(nxs_fw_ctl_c_rlfields_yesno(&_s_question_collection_del) == NXS_NO) {

		nxs_error(rc, NXS_FW_CTL_E_CANCEL, error);
	}

	if(nxs_fw_ctl_c_dir_clean(&objs_path) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_c_dir_del(&u_ctx->path) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_collections_make_sub_headers(u_ctx) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

error:

	nxs_string_free(&objs_path);

	return rc;
}

/* Module internal (static) functions */

static nxs_fw_ctl_err_t nxs_fw_ctl_u_collections_make_fs_struct(nxs_fw_ctl_u_collections_t *u_ctx)
{

	if(nxs_fs_mkdir(&u_ctx->path, NXS_FW_CTL_DIR_MODE_DEF) < 0) {

		if(errno == EEXIST) {

			nxs_log_write_error(&process, "can't create new collection: collection exist (collection path: %r)", &u_ctx->path);

			return NXS_FW_CTL_E_ERR;
		}
		else {

			nxs_log_write_error(
			        &process, "can't create collection fs structure: %s (creating dir: %r)", strerror(errno), &u_ctx->path);

			return NXS_FW_CTL_E_ERR;
		}
	}

	return NXS_FW_CTL_E_OK;
}

static nxs_fw_ctl_err_t nxs_fw_ctl_u_collections_make_makefiles(nxs_fw_ctl_u_collections_t *u_ctx, nxs_string_t *fw_version)
{
	nxs_string_t     tpl_path, dst_path;
	nxs_array_t      subs;
	nxs_fw_ctl_err_t rc;
	size_t           i;

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&tpl_path);
	nxs_string_init(&dst_path);

	nxs_fw_ctl_c_copy_tpl_init(&subs);

	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_c_filename, &u_ctx->name);
	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_c_rpath, &u_ctx->rpath);
	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_c_obj, &u_ctx->obj_name);

	for(i = 0; nxs_string_len(&makefiles[i].src) > 0; i++) {

		nxs_string_printf(&tpl_path,
		                  "%r/%r/" NXS_FW_CTL_DIR_ADD_TPL "/" NXS_FW_CTL_DIR_COLLECTIONS "/%r",
		                  &nxs_fw_ctl_cfg.tpls_path,
		                  fw_version,
		                  &makefiles[i].src);
		nxs_string_printf(&dst_path, "%r/%r", &u_ctx->path, &makefiles[i].dst);

		if(nxs_fw_ctl_c_copy_tpl(&subs, &tpl_path, &dst_path, makefiles[i].mode) != NXS_FW_CTL_E_OK) {

			nxs_error(rc, NXS_FW_CTL_E_ERR, error);
		}
	}

error:

	nxs_fw_ctl_c_copy_tpl_free(&subs);

	nxs_string_free(&tpl_path);
	nxs_string_free(&dst_path);

	return rc;
}

static nxs_fw_ctl_err_t nxs_fw_ctl_u_collections_make_genfiles(nxs_fw_ctl_u_collections_t *u_ctx, nxs_string_t *fw_version)
{
	nxs_string_t     tpl_path, dst_path;
	nxs_array_t      subs;
	nxs_fw_ctl_err_t rc;
	size_t           i;

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&tpl_path);
	nxs_string_init(&dst_path);

	nxs_fw_ctl_c_copy_tpl_init(&subs);

	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_c_filename, &u_ctx->name);
	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_c_rpath, &u_ctx->rpath);
	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_c_obj, &u_ctx->obj_name);
	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_c_upcase_name, &u_ctx->upcase_name);
	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_c_inline_name, &u_ctx->inline_name);
	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_c_p_name, &u_ctx->proj_name);
	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_c_p_inline_name, &u_ctx->proj_inline_name);

	for(i = 0; nxs_string_len(&genfiles[i].src) > 0; i++) {

		nxs_string_printf(&tpl_path,
		                  "%r/%r/" NXS_FW_CTL_DIR_ADD_TPL "/" NXS_FW_CTL_DIR_COLLECTIONS "/%r",
		                  &nxs_fw_ctl_cfg.tpls_path,
		                  fw_version,
		                  &genfiles[i].src);
		nxs_string_printf(&dst_path, "%r/%r", &u_ctx->path, &genfiles[i].dst);

		nxs_fw_ctl_c_copy_tpl_path(&subs, &dst_path);

		if(nxs_fw_ctl_c_copy_tpl(&subs, &tpl_path, &dst_path, genfiles[i].mode) != NXS_FW_CTL_E_OK) {

			nxs_error(rc, NXS_FW_CTL_E_ERR, error);
		}
	}

error:

	nxs_fw_ctl_c_copy_tpl_free(&subs);

	nxs_string_free(&tpl_path);
	nxs_string_free(&dst_path);

	return rc;
}

static nxs_fw_ctl_err_t nxs_fw_ctl_u_collections_make_sub_headers(nxs_fw_ctl_u_collections_t *u_ctx)
{
	nxs_array_t      sub_els;
	size_t           i;
	nxs_string_t     header_path, coll_headers, *s;
	nxs_fw_ctl_err_t rc;

	rc = NXS_FW_CTL_E_OK;

	nxs_array_init(&sub_els, 0, sizeof(nxs_string_t), 1, NULL, NULL);

	nxs_string_init(&header_path);

	/*
	 * Подготовка нового значения блока 'includes'
	 */

	nxs_string_init2(&coll_headers, 0, (u_char *)NXS_FW_CTL_U_COLLS_TPL_C_HEADERS_B "\n");

	if(nxs_fw_ctl_c_dir_get_name_dirs(&u_ctx->ppath, &sub_els, NULL) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	for(i = 0; i < nxs_array_count(&sub_els); i++) {

		s = nxs_array_get(&sub_els, i);

		nxs_string_printf2_cat(&coll_headers, "#include <collections/%r/%r.h>\n", s, s);
	}

	nxs_string_char_cat(&coll_headers, (u_char *)NXS_FW_CTL_U_COLLS_TPL_C_HEADERS_E);

	/*
	 * Процесс замещения блока 'includes'
	 */

	nxs_string_printf(&header_path, "%rsrc/%r-collections.h", &u_ctx->proj_root, &u_ctx->proj_name);

	/* headers */

	if(nxs_fw_ctl_c_subs_includes(&header_path, NXS_YES, &coll_headers, &_s_tpl_c_headers_b, &_s_tpl_c_headers_e) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

error:

	nxs_string_free(&header_path);
	nxs_string_free(&coll_headers);

	for(i = 0; i < nxs_array_count(&sub_els); i++) {

		s = nxs_array_get(&sub_els, i);

		nxs_string_free(s);
	}

	nxs_array_free(&sub_els);

	return rc;
}
