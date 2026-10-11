import { execFileSync } from 'node:child_process';
import { readFileSync, writeFileSync, mkdirSync, cpSync } from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';
import { marked } from 'marked';
import sanitizeHtml from 'sanitize-html';
import katex from 'katex';
let formulas = [];
function mathToken(source, block) {
  const match = block
    ? /^(?:\$\$([\s\S]+?)\$\$|\\\[([\s\S]+?)\\\])(?:\n|$)/.exec(source)
    : /^(?:\$\$([\s\S]+?)\$\$|\$([^\n$]+?)\$|\\\(([\s\S]+?)\\\)|\\\[([\s\S]+?)\\\])/.exec(source);
  if (!match) return;
  return {type:block ? 'mathBlock' : 'mathInline', raw:match[0], formula:match.slice(1).find(value=>value !== undefined), display:block || match[0].startsWith('$$') || match[0].startsWith('\\[')};
}
function mathPlaceholder(token) {
  const id = formulas.length;
  formulas.push(katex.renderToString(token.formula, {displayMode:token.display, throwOnError:false, trust:false, strict:'ignore', output:'htmlAndMathml'}));
  return `ZMJFORMULAPLACEHOLDER${id}END`;
}
marked.use({extensions:[
  {name:'mathBlock', level:'block', start:source=>source.search(/\$\$|\\\[/), tokenizer:source=>mathToken(source,true), renderer:token=>`<div>${mathPlaceholder(token)}</div>\n`},
  {name:'mathInline', level:'inline', start:source=>source.search(/\$|\\[([]/), tokenizer:source=>mathToken(source,false), renderer:mathPlaceholder}
]});
const root = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '../..');
const git = (...args) => execFileSync('git', ['-c', 'core.quotepath=false', ...args], {cwd: root, encoding: 'utf8', maxBuffer: 20 * 1024 * 1024});
const files = git('ls-files', '-z').split('\0').filter(p => /\.md$/i.test(p) && !/^(docs|tools|\.github|\.codex|\.agents)\//.test(p) && !/^(README|WORKFLOW|AGENTS)\.md$/i.test(p));
const history = git('log', '--format=DATE:%cs', '--name-only', '--', '*.md').split('\n');
const dates = new Map(); let date = '';
for (const line of history) { if (line.startsWith('DATE:')) date = line.slice(5); else if (line) { let name = line; if (name.startsWith('"')) continue; if (!dates.has(name)) dates.set(name, date); } }
function resolveLink(value, file, image = false) {
  if (/^(https?:|mailto:|#)/i.test(value)) return value;
  if (/^[a-z][a-z\d+.-]*:/i.test(value)) return '';
  const normalized = path.posix.normalize(path.posix.join(path.posix.dirname(file), value.replace(/\\/g, '/')));
  if (normalized.startsWith('../')) return '';
  const encoded = normalized.split('/').map(encodeURIComponent).join('/');
  return image ? `https://raw.githubusercontent.com/1995zmj/ZMJ/master/${encoded}` : `https://github.com/1995zmj/ZMJ/blob/master/${encoded}`;
}
const articles = files.map(file => {
  const markdown = readFileSync(path.join(root, file), 'utf8').replace(/^\uFEFF/, '');
  const title = markdown.match(/^#\s+(.+)$/m)?.[1]?.trim() || path.posix.basename(file, path.posix.extname(file));
  formulas = [];
  let html = sanitizeHtml(marked.parse(markdown), {allowedTags: sanitizeHtml.defaults.allowedTags.concat(['img']), allowedAttributes: {...sanitizeHtml.defaults.allowedAttributes, img: ['src','alt','title','loading'], a:['href','title']}, transformTags: {
    a: (tag, attrs) => ({tagName:tag, attribs:{...attrs, href:resolveLink(attrs.href || '', file)}}),
    img: (tag, attrs) => ({tagName:tag, attribs:{...attrs, src:resolveLink(attrs.src || '', file, true), loading:'lazy'}})
  }});
  html = html.replace(/ZMJFORMULAPLACEHOLDER(\d+)END/g, (_, index) => formulas[Number(index)]);
  const text = sanitizeHtml(html, {allowedTags:[], allowedAttributes:{}}).replace(/\s+/g,' ').trim();
  return {id:file, title, category:file.includes('/') ? file.split('/')[0] : '其他', date: dates.get(file) || git('log','-1','--format=%cs','--',file).trim(), summary:text.slice(0,120), text, html, url:`https://github.com/1995zmj/ZMJ/blob/master/${file.split('/').map(encodeURIComponent).join('/')}`};
}).filter(article => article.html.trim()).sort((a,b) => b.date.localeCompare(a.date) || a.title.localeCompare(b.title,'zh-CN'));
writeFileSync(path.join(root,'docs/articles.js'), `window.BLOG_ARTICLES = ${JSON.stringify(articles).replace(/</g,'\\u003c')};\n`);
console.log(`Generated ${articles.length} articles across ${new Set(articles.map(a=>a.category)).size} categories.`);

mkdirSync(path.join(root, 'docs/math'), {recursive:true});
cpSync(path.join(root,'tools/blog/node_modules/katex/dist/katex.min.css'), path.join(root,'docs/math/katex.min.css'));
cpSync(path.join(root,'tools/blog/node_modules/katex/dist/fonts'), path.join(root,'docs/math/fonts'), {recursive:true});